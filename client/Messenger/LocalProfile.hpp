#ifndef LIGHTYEAR_LOCALPROFILE_HPP
#define LIGHTYEAR_LOCALPROFILE_HPP
#pragma  once
#include <vector>
#include <deque>
#include <memory> // For std::shared_ptr
#include <string>
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"
#include "chat/ChatMessage.hpp"
#include "include/base/cef_lock.h"

class LocalProfile :public CefBase {
public:
    static LocalProfile* Instance();
    std::vector<std::shared_ptr<RichChatMessage>> QueryChatMessagesById(std::wstring id);
    void AddChatMessage(const std::shared_ptr<RichChatMessage> msg);
    void set_server_ip(std::wstring server_ip) {
        server_ip_ = server_ip;
    }
    std::wstring ServerIP() {
        return server_ip_;
    }
private:
    LocalProfile();
    std::vector<std::shared_ptr<RichChatMessage>> m_chatMessages;
    base::Lock  m_chatMessagesLock;

    std::wstring server_ip_;
    static LocalProfile* s_Instance;
    IMPLEMENT_REFCOUNTING(LocalProfile);
};
#endif