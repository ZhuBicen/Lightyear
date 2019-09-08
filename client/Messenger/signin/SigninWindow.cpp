#include "signin/SigninWindow.hpp"
#include "sc/StateChart.hpp"
#include "proto/src/core.pb.h"
#include "UiMessage.hpp"
#include "MessageQueue.hpp"
#include "Utility.hpp"
#include "RosterMgmt.hpp"
#include "MessageBuilder.hpp"
#include "CommandLine.hpp"
#include "Log.hpp"
#include "UiMessage.hpp"
#include "about/AboutWindow.hpp"

SigninWindow::SigninWindow(MessageQueue& p_messageQueue, MainStateChart& p_mainStateChart):
    m_messageQueue(p_messageQueue), m_timerId(100),
    m_mainStateChart(&p_mainStateChart) {
}

SigninWindow::~SigninWindow() {
}

void SigninWindow::InitWindow(){
    ::SetWindowText(GetHWND(), GetWindowText());
    SetIcon(IDR_MAINFRAME);

    m_infoLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("infoLabel")));
    m_signinButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("signinButton")));
    m_accountEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("accountedit")));
    m_pwEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("pwedit")));

    auto l_autoSigninInfo = CommandLine::Instance()->GetAutoSignin();
    if (std::get<0>(l_autoSigninInfo)) {
        auto id = std::get<1>(l_autoSigninInfo);
        auto pw = std::get<2>(l_autoSigninInfo);
        m_accountEdit->SetText(id.c_str());
        m_pwEdit->SetText(pw.c_str());
    }
    m_PaintManager.SetTimer(m_signinButton, m_timerId, 1000);
};

LRESULT SigninWindow::ResponseDefaultKeyEvent(WPARAM wParam) {
    if (wParam == VK_RETURN) {
        Signin();
        return TRUE;
    }
    return FALSE;
}
LRESULT SigninWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    if (uMsg == WM_INTERNAL_MESSAGE) {
        bHandled = true;
        m_mainStateChart->ProcessEvent(std::shared_ptr<Event>((Event*)wParam));
        return 0;
    }
	return 0;
}

void SigninWindow:: Notify(TNotifyUI& msg) {
	if (_tcsicmp(msg.sType, _T("click")) == 0) {
        if (_tcsicmp(msg.pSender->GetName(), _T("signinButton")) == 0) {
            Signin();
            return;
        }
        if (_tcsicmp(msg.pSender->GetName(), _T("closebtn")) == 0) {
            Close();
            m_mainStateChart->ProcessEvent(std::make_shared<Event>(MessageType::SigninExit));
            return;
        }
        if (_tcsicmp(msg.pSender->GetName(), _T("about")) == 0) {
            ShowAboutWindow();
            return;
        }
    }
    if (_tcsicmp(msg.sType, DUI_MSGTYPE_TIMER) == 0) {
        m_PaintManager.KillTimer(m_signinButton, m_timerId);
        m_mainStateChart->ProcessEvent(std::make_shared<Event>(MessageType::SelfChecking));
        return;
    }
    __super::Notify(msg);
	
}
void SigninWindow::Signin()
{
    if (m_accountEdit->GetText().IsEmpty()) {
        m_infoLabel->SetText(_T("请输入您的工号！"));
        m_infoLabel->SetVisible();
        return;
    }
    if (m_pwEdit->GetText().IsEmpty()) {
        m_infoLabel->SetText(_T("请输入您的密码！"));
        m_infoLabel->SetVisible();
        return;
    }
    m_signinButton->SetEnabled(false);
    m_mainStateChart->ProcessEvent(std::make_shared<Event>(MessageType::StartSignin));
}
