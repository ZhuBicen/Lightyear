#pragma once
#include <memory> // For shared_ptr
#include "proto/src/im.pb.h"

class RichChatMessage;

namespace core{
	class Jid;
}
class MessageBuilder
{
public:
	MessageBuilder(void);
	~MessageBuilder(void);
	static google::protobuf::MessageLite* BuildRosterReq() ;
    static google::protobuf::MessageLite* BuildAuthenticationReq(const std::wstring& p_id, const std::wstring& p_pw);
    static google::protobuf::MessageLite* BuildPresence(const im::Presence::Show& presenceStatus);
    static google::protobuf::MessageLite* BuildChatMessage(const std::shared_ptr<RichChatMessage> p_chatMessage);

	static google::protobuf::MessageLite* BuildJingleMessage(const core::Jid& p_fromJid, const core::Jid& p_toJid, const std::string& p_jsonMessage);
    static std::shared_ptr<RichChatMessage>  ToInternalChatMessage(const im::ChatMessage* p_message);
    static google::protobuf::MessageLite* BuildHeartbeatAck();
};

