// excerpts from http://code.google.com/p/muduo/
//               http://blog.csdn.net/solstice/article/details/6300108
//
#include <boost/crc.hpp>  
#include "proto/Codec.hpp"
#include "Log.hpp"
#include "proto/src/core.pb.h"
#include "proto/src/im.pb.h"
#include "proto/src/p2p.pb.h"
// #include <zlib.h>  // adler32

#include <string>
#include <stdint.h>
#include <Winsock2.h> // for htonl
// #pragma comment(lib, "libprotobuf.lib")
// struct ProtobufTransportFormat __attribute__ ((__packed__))
// {
//   int32_t  len;
//   int32_t  nameLen;
//   char     typeName[nameLen];
//   char     protobufData[len-nameLen-8];
//   int32_t  checkSum; // adler32 of nameLen, typeName and protobufData
// }

const int kHeaderLen = sizeof(int32_t);

///
/// CRC-32
/// 
int32_t Crc32(const char* bytes, int32_t length) {
    boost::crc_32_type result;
    result.process_bytes(bytes, length);
    return result.checksum();
}

///
/// Encode protobuf Message to transport format defined above
/// returns a std::string.
///
/// returns a empty string if message.AppendToString() fails.
///
std::string encode(const google::protobuf::MessageLite& message)
{
	std::string result;

	result.resize(kHeaderLen);

	const std::string& typeName = message.GetTypeName();
	int32_t nameLen = static_cast<int32_t>(typeName.size()+1);
	int32_t be32 = ::htonl(nameLen);
	result.append(reinterpret_cast<char*>(&be32), sizeof be32);
	result.append(typeName.c_str(), nameLen);
	bool succeed = message.AppendToString(&result);

	if (succeed)
	{
		const char* begin = result.c_str() + kHeaderLen;
		int32_t checkSum = Crc32(begin, result.size() - kHeaderLen);
		int32_t be32 = ::htonl(checkSum);
		result.append(reinterpret_cast<char*>(&be32), sizeof be32);
		// MLOG(logINFO) << __FUNCTION__ << "() encoding Packet len : " << result.size() - kHeaderLen;
		int32_t len = ::htonl(result.size() - kHeaderLen);
		std::copy(reinterpret_cast<char*>(&len),
			reinterpret_cast<char*>(&len) + sizeof len,
			result.begin());
	}
	else
	{
		result.clear();
	}

	return result;
}

google::protobuf::MessageLite* createMessage(const std::string& type_name)
{
	if (type_name == "core.AuthenticationReq") {
		return new core::AuthenticationReq();
	}
	if (type_name == "core.AuthenticationResp") {
		return new core::AuthenticationResp();
	}
	if (type_name == "im.Presence") {
		return new im::Presence();
	}
	if (type_name == "im.PresenceReq") {
		return new im::PresenceReq();
	}
	if (type_name == "im.PresenceResp") {
		return new im::PresenceResp();
	}
	if (type_name == "im.RosterReq") {
		return new im::RosterReq();
	}
	if (type_name == "im.RosterResp") {
		return new im::RosterResp();
	}
	if (type_name == "im.ChatMessage") {
		return new im::ChatMessage();
	}
	if (type_name == "p2p.JingleMessage") {
		return new p2p::JingleMessage();
	}
    if (type_name == "core.Heartbeat") {
        return new core::Heartbeat();
    }
	MLOG(logERROR) << "Unkown message received: " << type_name;
	return 0;
}

int32_t asInt32(const char* buf)
{
	int32_t be32 = 0;
	::memcpy(&be32, buf, sizeof(be32));
	return ::ntohl(be32);
}

///
/// Decode protobuf Message from transport format defined above.
/// returns a Message*
///
/// returns NULL if fails.
///
google::protobuf::MessageLite* decode(const std::string& buf)
{
	google::protobuf::MessageLite* result = NULL;

	int32_t len = static_cast<int32_t>(buf.size());
	if (len >= 10)
	{
		int32_t expectedCheckSum = asInt32(buf.c_str() + buf.size() - kHeaderLen);
		const char* begin = buf.c_str();
		int32_t checkSum = Crc32(begin, len - kHeaderLen);
		if (checkSum == expectedCheckSum)
		{
			int32_t nameLen = asInt32(buf.c_str());
			if (nameLen >= 2 && nameLen <= len - 2*kHeaderLen)
			{
				std::string typeName(buf.begin() + kHeaderLen, buf.begin() + kHeaderLen + nameLen - 1);
				google::protobuf::MessageLite* message = createMessage(typeName);
				if (message)
				{
					const char* data = buf.c_str() + kHeaderLen + nameLen;
					int32_t dataLen = len - nameLen - 2*kHeaderLen;
					if (message->ParseFromArray(data, dataLen))
					{
						result = message;
					}
					else
					{
						// parse error
						MLOG(logINFO) << __FUNCTION__ << " Parse error";
						delete message;
					}
				}
				else
				{
					MLOG(logINFO) << __FUNCTION__ << " Unknown message type";
					// unknown message type
				}
			}
			else
			{
				MLOG(logINFO) << __FUNCTION__ << " Invalid name len";
				// invalid name len
			}
		}
		else
		{
			MLOG(logINFO) << __FUNCTION__ << " Checksum error";
			// check sum error
		}
	}

	return result;
}

