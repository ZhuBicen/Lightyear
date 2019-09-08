#ifndef LIGHTYEAR_TUP_SOCKET_HPP
#define LIGHTYEAR_TUP_SOCKET_HPP
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
class MessageQueue;
class Event;

class TcpSocket {
public:
	TcpSocket(MessageQueue& mesasgeQueue);
	void ReadFunc();
	void WriteFunc();

	// underlying problem, not thread safe.
	void SetHwnd(HWND hwnd) {
		hwnd_ = hwnd;
	}

    ~TcpSocket() {
        TerminateThread(write_thread_.native_handle(), -1);
        TerminateThread(read_thread_.native_handle(), -1);
    }
private:
    void PostInternalMessage(Event* event);
private:

    // To notify the read thread to start reading
	boost::mutex con_mutex_;
	boost::condition_variable_any condition_;

    // To communicate with the GUI thread
	MessageQueue& message_queue_;
	HWND hwnd_;    
    
	boost::asio::io_service io_service_; // io_service should declared/initialized before socket_
	boost::asio::ip::tcp::socket socket_;
    bool socket_connected_;   //dirty flag

    // TCP server
    boost::asio::ip::tcp::endpoint tcp_server_;

private:
    boost::thread write_thread_;
    boost::thread read_thread_;
	
};

#endif