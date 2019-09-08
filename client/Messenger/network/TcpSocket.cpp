#include <assert.h>
#include "network/TcpSocket.hpp"
#include "MessageQueue.hpp"
#include "proto/Codec.hpp"
#include "proto/src/core.pb.h"
#include "proto/src/im.pb.h"
#include "UiMessage.hpp"
#include "Log.hpp"
#include <boost/foreach.hpp>
#include "base/StateChartBase.hpp"
#include "LocalProfile.hpp"
#include "Utility.hpp"

using boost::asio::ip::tcp;
TcpSocket::TcpSocket(MessageQueue& messageQueue):message_queue_(messageQueue),
	socket_(io_service_), socket_connected_(false) {
    try {
        boost::asio::ip::tcp::resolver l_resolver(io_service_);  
        boost::asio::ip::tcp::resolver::query l_query("zhubicen.f3322.org", "8089"); 
        boost::asio::ip::tcp::resolver::iterator l_endpoint_iterator = l_resolver.resolve(l_query);
        boost::asio::ip::tcp::resolver::iterator l_end;
        if (l_endpoint_iterator != l_end) {
            tcp_server_ = *l_endpoint_iterator;
        }
    }
    catch (...) {
        tcp_server_ = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8089);
    }
     MLOG(logINFO) << " TCP Server IP:" << tcp_server_;
     LocalProfile::Instance()->set_server_ip(ToWstring(tcp_server_.address().to_string()));
     write_thread_ = boost::thread(boost::bind(&TcpSocket::WriteFunc, this));
     read_thread_ = boost::thread(boost::bind(&TcpSocket::ReadFunc, this));
}

void TcpSocket::ReadFunc(){
	{
		boost::mutex::scoped_lock lock(con_mutex_);
		condition_.wait(con_mutex_); 
	}

	for(;;) {

		int32_t l_len = 0;
		boost::system::error_code error;
		boost::asio::read(socket_, boost::asio::buffer(&l_len, sizeof(l_len)), error);
		if (error) {
            // server close the connection, reconnecting here ?
			PostInternalMessage(new Event(MessageType::NetworkError));
			MLOG(logERROR) << __FUNCTION__ << "(), Asio Read error:" << error;
			return;
		}
		l_len = ntohl(l_len); 
		// MLOG(logINFO) << __FUNCTION__ << "(), Received packet data length:" << l_len;
		char* l_rawData = new char[l_len];
		boost::asio::read(socket_, boost::asio::buffer(l_rawData, l_len), error);
		if (error) {
            PostInternalMessage(new Event(MessageType::NetworkError));
			MLOG(logERROR) << __FUNCTION__ << "(), Asio Read error:" << error;
			delete[] l_rawData;
			return;
		}
		std::string l_messageData;
		l_messageData.append(l_rawData, l_len);
		google::protobuf::MessageLite* l_gMessage = decode(l_messageData);
        if (l_gMessage == nullptr) {
            MLOG(logINFO) << __FUNCTION__ << " Invalid message received.";
            continue;
        }
		MLOG(logINFO) << __FUNCTION__ << "(), Message received:" << l_gMessage->GetTypeName();
        // How to set the size of the proto buffer message
        PostInternalMessage(new Event(MessageType::Protobuffer,l_gMessage));
		delete l_rawData;
	}
}

void TcpSocket::WriteFunc(){

	for(;;) {
		Message* l_message;
		l_message = message_queue_.GetMessage();
        if (l_message == nullptr) {
            MLOG(logINFO) << __FUNCTION__ << " exit.";
            return;
        }
        std::string l_messageString = encode(*l_message);
        // TODO: Support reconnecting
		if (!socket_connected_ && l_message->GetTypeName() == "core.AuthenticationReq") {
			boost::system::error_code ec;
			socket_.connect(tcp_server_, ec);
			if (ec) {
				PostInternalMessage(new Event(MessageType::NetworkError));
				MLOG(logERROR) << __FUNCTION__ << "(), Asio Connect error:" << ec ;
				continue; // waiting for the re-connection
			}
            socket_connected_ = true;
			condition_.notify_one();
		}
		boost::system::error_code error;
		boost::asio::write(socket_, boost::asio::buffer(l_messageString), error);
		if (error == boost::asio::error::eof){
			PostInternalMessage(new Event(MessageType::NetworkError));
			MLOG(logERROR) << __FUNCTION__ << "(), Connection closed cleanly by peer.";
		}else if (error) {
            PostInternalMessage(new Event(MessageType::NetworkError));
			MLOG(logERROR) << __FUNCTION__ << "(), Asio Write error:" << error;
		}
		delete l_message;
	}
}

void TcpSocket::PostInternalMessage(Event* event) {
    if (::IsWindow(hwnd_)) {
        ::PostMessage(hwnd_, WM_INTERNAL_MESSAGE, (WPARAM)event, 0);
        return;
    }
    MLOG(logWARNING) << __FUNCTION__ << "Discarding event:" << (int)((Event*)event)->Type();
}
