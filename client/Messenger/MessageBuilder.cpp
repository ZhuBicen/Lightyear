#include "MessageBuilder.hpp"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include "RosterMgmt.hpp"
#include "Utility.hpp"
#include "chat/ChatMessage.hpp"
#include "proto/src/p2p.pb.h"
#include "Log.hpp"

MessageBuilder::MessageBuilder(void)
{
}


MessageBuilder::~MessageBuilder(void)
{
}

google::protobuf::MessageLite*
MessageBuilder:: BuildRosterReq( ) {
	auto l_rr = new im::RosterReq();
	auto l_jid = new core::Jid;
	l_jid->set_id(ToString(RosterMgmt::Instance()->Myself().id));
	l_jid->set_resource("WINPC");
	l_rr->set_allocated_jid(l_jid);
	return l_rr;

}

google::protobuf::MessageLite*
MessageBuilder::BuildAuthenticationReq(const std::wstring& p_id, const std::wstring& p_pw)
{		
    core::AuthenticationReq* l_aq = new core::AuthenticationReq;
    core::Jid* l_jid = new core::Jid;
    l_jid->set_id(ToString(p_id));
    l_jid->set_resource("WINPC");
    l_aq->set_allocated_jid(l_jid);
    l_aq->set_password(ToString(p_pw));
    return l_aq;
}
google::protobuf::MessageLite* 
MessageBuilder::BuildPresence(const im::Presence::Show& presenceStatus) {
    im::Presence* l_presence = new im::Presence;
    l_presence->set_show(presenceStatus);
    auto l_jid = new core::Jid;
    l_jid->set_id(ToString(RosterMgmt::Instance()->Myself().id));
    l_jid->set_resource(ToString(RosterMgmt::Instance()->GetMyselfResource()));

    l_presence->set_allocated_fromjid(l_jid);
    return l_presence;
}

google::protobuf::MessageLite* 
MessageBuilder::BuildChatMessage(const std::shared_ptr<RichChatMessage> p_chatMessage)
{
    auto l_chatMessage = new im::ChatMessage;
    auto l_fromJid = new core::Jid;
    l_fromJid->set_id(ToString(p_chatMessage->From()));
    l_fromJid->set_resource(ToString(RosterMgmt::Instance()->GetMyselfResource()));

    auto l_toJid = new core::Jid;
    l_toJid->set_id(ToString(p_chatMessage->To()));

    l_chatMessage->set_allocated_fromjid(l_fromJid);
    l_chatMessage->set_allocated_tojid(l_toJid);

    BOOST_FOREACH (MessageElement* l_element, p_chatMessage->Elements())
    {
      im::MessageElement* l_messageElement = l_chatMessage->add_body();
      l_messageElement->set_kind(l_element->Kind());
      l_messageElement->set_data(l_element->Data());
    }


    return l_chatMessage;
}

google::protobuf::MessageLite* 
MessageBuilder::BuildJingleMessage(const core::Jid& p_fromJid, const core::Jid& p_toJid, const std::string& p_jsonMessage) {
	auto l_jingleMessage = new p2p::JingleMessage;
	auto l_fromJid = new core::Jid(p_fromJid);
	auto l_toJid = new core::Jid(p_toJid);

	l_jingleMessage->set_allocated_fromjid(l_fromJid);
	l_jingleMessage->set_allocated_tojid(l_toJid);
	l_jingleMessage->set_content(p_jsonMessage);
	return l_jingleMessage;
}

std::shared_ptr<RichChatMessage>
MessageBuilder::ToInternalChatMessage(const im::ChatMessage* p_message)
{
    std::wstring l_fromId = ToWstring(p_message->fromjid().id());
    std::wstring l_toId = ToWstring(p_message->tojid().id());

    auto l_chatMessage = std::make_shared<RichChatMessage>();
    for (auto l_i = 0; l_i < p_message->body_size(); l_i++) {
        auto l_messageElement = p_message->body(l_i);
        if (l_messageElement.kind() == im::TEXT) {
            l_chatMessage->push(new PlainText(ToWstring(l_messageElement.data())));
        }
        else if (l_messageElement.kind() == im::SMILEY) {
            l_chatMessage->push(new Smiley(ToWstring(l_messageElement.data())));
        }
        else if (l_messageElement.kind() == im::BITMAP) {
            l_chatMessage->push(new ImageElement(l_messageElement.data()));
        }
        else {
            MLOG(logERROR) << "Invalid message element found.";
        }
    }
    l_chatMessage->SetFrom(l_fromId);
    l_chatMessage->SetTo(l_toId);
    return l_chatMessage;

}

google::protobuf::MessageLite*
MessageBuilder::BuildHeartbeatAck() {
    auto ha = new core::HeartbeatAck;
    auto l_fromJid = new core::Jid;
    l_fromJid->set_id(ToString(RosterMgmt::Instance()->Myself().id));
    l_fromJid->set_resource(ToString(RosterMgmt::Instance()->GetMyselfResource()));
    ha->set_allocated_fromjid(l_fromJid);
    return ha;
}
