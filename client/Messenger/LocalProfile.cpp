#include "LocalProfile.hpp"
#include "RosterMgmt.hpp"
#include "MessageBuilder.hpp"
#include "log.hpp"
#include "utility.hpp"
#include <google/protobuf/io/coded_stream.h>
#include "proto/src/localdb.pb.h"

#include <fstream>

LocalProfile* LocalProfile::s_Instance = nullptr;

LocalProfile::LocalProfile() {
}
LocalProfile* LocalProfile::Instance() {
    if (s_Instance == nullptr) {
        s_Instance = new LocalProfile();
    }
    return s_Instance;
}

std::vector<std::shared_ptr<RichChatMessage>> LocalProfile::QueryChatMessagesById(std::wstring id){
    base::AutoLock lockScope(m_chatMessagesLock);
    std::vector<std::shared_ptr<RichChatMessage>> l_msgs;
    auto l_myId = RosterMgmt::Instance()->Myself().id;
    for (auto l_msg: m_chatMessages) {
        if ((l_msg->From() == l_myId  && l_msg->To() == id) ||
            (l_msg->From() == id      && l_msg->To() == l_myId)) {
            l_msgs.push_back(l_msg);
        }
    }
    return l_msgs;
}


void LocalProfile::AddChatMessage(const std::shared_ptr<RichChatMessage> msg) {
    base::AutoLock lockScope(m_chatMessagesLock);
    m_chatMessages.push_back(msg);
}

