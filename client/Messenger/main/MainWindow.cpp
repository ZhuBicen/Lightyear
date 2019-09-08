#include "main/MainWindow.hpp"
#include "chat/ChatWindow.hpp"
#include "chat/ChatMessage.hpp"
#include "video/VideoWindow.hpp"
#include "about/AboutWindow.hpp"

#include <mmsystem.h> // For PlaySound
#include "LocalProfile.hpp"
#include "MessageBuilder.hpp"
#include "MessageQueue.hpp"
#include "UiMessage.hpp"
#include "Utility.hpp"
#include "RosterMgmt.hpp"
#include "resource.h"
#include "sc/StateChart.hpp"
#include "control/RosterLogoButton.hpp"
#include "control/RosterContextMenu.hpp"
#include "control/RecentRosterList.hpp"
#include "control/TrayMenu.hpp"
#include "proto/src/p2p.pb.h"
#include "proto/src/im.pb.h"
#include "Log.hpp"
#pragma  comment (lib, "winmm.lib")

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

const TCHAR* const kFriendsListControlName = _T("friends");
const TCHAR* const kMyselfControlName = _T("myselfName");
const TCHAR* const kSignatureTipsControlName = _T("signaturetip");
const TCHAR* const kSignatureControlName = _T("signature");

const TCHAR* const kSearchEditTipControlName = _T("search_tip");
const TCHAR* const kSearchEditControlName = _T("search_edit");

const TCHAR* const kChangeBkSkinControlName = _T("bkskinbtn");
const TCHAR* const kChangeColorSkinControlName = _T("colorskinbtn");

const TCHAR* const kBackgroundControlName = _T("bg");

const TCHAR* const kRosterListOptionName = _T("rosterListOption");
const TCHAR* const kRecentRosterListOptionName = _T("recentRosterListOption");

const int kBackgroundSkinImageCount = 3;

MainWindow::MainWindow(MessageQueue& p_MessageQueue, MainStateChart* p_mainStateChart):
m_messageQueue(p_MessageQueue),
#ifdef ENABLE_GOOGLE_WEBRTC
m_videoWindow(nullptr),
#endif
m_mainStateChart(p_mainStateChart)
{
    std::wstring name;
    if (RosterMgmt::Instance()->Myself().name) {
        name = *RosterMgmt::Instance()->Myself().name;
    }
    m_trayIcon = new CTrayIcon(name.c_str(), true, 
        LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)));
    m_trayIcon->SetListener(this);
}

MainWindow::~MainWindow()
{
	// PostQuitMessage(0);
}

LPCTSTR MainWindow::GetWindowClassName() const
{
	return _T("LIGHTYEAR_MAIN_WINDOW");
}

CControlUI* MainWindow::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("RosterList")) == 0)
	{
		return new CRosterList(m_PaintManager);
	}
    if (_tcsicmp(pstrClass, _T("RecentRosterList")) == 0) {
        return new CRecentRosterList(m_PaintManager);
    }

	return NULL;
}

void MainWindow::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString MainWindow::GetSkinFile() const
{
	return _T("xml\\MainWindow.xml");
}


LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}
LRESULT MainWindow::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void MainWindow::OnTimer(TNotifyUI& msg)
{
    if (msg.wParam == HEARTBEAT_TIMER_ID) {
        m_mainStateChart->ProcessEvent(std::make_shared<Event>(Event(MessageType::HearbeatTimeout)));
        return;
    }
    MLOG(logWARNING) << "Unrecognized timer message";
}

void MainWindow::OnExit(TNotifyUI& msg)
{
	Close();
}

void MainWindow::InitWindow(){
    SetIcon(IDR_MAINFRAME);
    // init the roster
    m_rosterList = static_cast<CRosterList*>(m_PaintManager.FindControl(_T("rosterList")));
    m_recentRosterList = static_cast<CRecentRosterList*>(m_PaintManager.FindControl(_T("recentRosterList")));
    m_presenceStatusBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"presenceStatusBtn"));
    for (auto roster : RosterMgmt::Instance()->GetRoster()) {
        if (roster.id != RosterMgmt::Instance()->Myself().id){
            m_rosterList->AddRosterItem(roster);
        }
    }

    // init the recent contact
    //for (auto roster : LocalProfile::Instance()->GetRecentRosterItems()) {
    //    m_recentRosterList->AddRecentRosterItem(RosterMgmt::Instance()->GetRosterItem(roster));
    //}
    auto l_myself = RosterMgmt::Instance()->Myself();
    auto l_myselfNameCtl = static_cast<CListUI*>(m_PaintManager.FindControl(kMyselfControlName));
    if (l_myself.name) {
        l_myselfNameCtl->SetText((*l_myself.name).c_str());
    }
    else {
        //如果没有名字？，显示ID
        l_myselfNameCtl->SetText(l_myself.id.c_str());
    }

    // init the menu
    {
        auto hMenu = ::LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU2));
        m_presenceStatusMenu = GetSubMenu(hMenu, 0);
    }
    // init the tray menu
    {
        auto hmenu = ::LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
        m_trayMenu = GetSubMenu(hmenu, 0);
    }
}

DWORD MainWindow::GetBkColor()
{
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
		return background->GetBkColor();

	return 0;
}

void MainWindow::SetBkColor(DWORD dwBackColor)
{
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		background->SetBkImage(_T(""));
		background->SetBkColor(dwBackColor);
		background->NeedUpdate();

        // TODO: broadcast the theme change event.
	}
}

void MainWindow::UpdateFriendsList()
{
	//if (pFriendsList->GetCount() > 0)
	//	pFriendsList->RemoveAll();
}


void MainWindow::OnPrepare(TNotifyUI& msg)
{
}

void MainWindow::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kSignatureControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* signature_tip = m_PaintManager.FindControl(kSignatureTipsControlName);
			if (signature_tip != NULL)
			{
				CRichEditUI* signature = static_cast<CRichEditUI*>(msg.pSender);
				if (signature != NULL)
					signature_tip->SetText(signature->GetText());
				signature_tip->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* search_tip = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSearchEditTipControlName));
			if (search_tip != NULL)
			{
				CRichEditUI* search_edit = static_cast<CRichEditUI*>(msg.pSender);
				if (search_edit != NULL)
					search_tip->SetText(search_edit->GetText());
				search_tip->SetVisible(true);
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
        if (_tcsicmp(msg.pSender->GetName(), kSignatureTipsControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* signature = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSignatureControlName));
			if (signature != NULL)
			{
				signature->SetText(msg.pSender->GetText());
				signature->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditTipControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* search_edit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSearchEditControlName));
			if (search_edit != NULL)
			{
				search_edit->SetText(msg.pSender->GetText());
				search_edit->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kChangeBkSkinControlName) == 0)
        {
            LightyearWinodwBase::ChangeTheme();
            this->UpdateTheme();
            for (auto chatWindow: m_chatDialogs) {
                chatWindow.second->UpdateTheme();
            }
#ifdef ENABLE_GOOGLE_WEBRTC
            if (m_videoWindow != nullptr) {
                m_videoWindow->UpdateTheme();
            }
#endif
        }
        else if (msg.pSender->GetName() == _T("presenceStatusBtn")) {
            // CheckMenuItem(m_presenceStatusMenu, ID_PRESENCE_ONLINE, MF_CHECKED);
            POINT point;
            GetCursorPos(&point);
            TrackPopupMenu(m_presenceStatusMenu, TPM_RIGHTBUTTON, point.x, point.y,
                0, m_hWnd, NULL);
            return;
        }
        else if(msg.pSender->GetName() == _T("minbtn")) {
            ::ShowWindow(m_hWnd, SW_HIDE);
        }
        else if (msg.pSender->GetName() == _T("closebtn")) {
            ::ShowWindow(m_hWnd, SW_HIDE);
            // https://code.csdn.net/ZhuBC/lightyear/issues/3
            // m_trayIcon->ShowBalloonTooltip(_T("Lightyear"), _T("Lightyear正在后台运行"));
            return;
        }
        else if (msg.pSender->GetName() == _T("3rd_logs")) {
            ::ShellExecute(0, 0, L"http://log.xawinway.cn/index.jsp", 0, 0, SW_SHOW);
            return;
        }
        else if (msg.pSender->GetName() == _T("3rd_mes")) {
            ::ShellExecute(0, 0, L"http://www.163.com", 0, 0, SW_SHOW);
            return;
        }
        else if (msg.pSender->GetName() == _T("3rd_stock")) {
            ::ShellExecute(0, 0, L"http://www.sina.com", 0, 0, SW_SHOW);
            return;
        }
        else if (msg.pSender->GetName() == _T("3rd_sale")) {
            ::ShellExecute(0, 0, L"http://s.xawinway.cn:8090/loginActionCAS.do?ffc=loginCAS", 0, 0, SW_SHOW);
            return;
        }
        else {
            __super::OnClick(msg);
        }
	}
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
    else if (msg.sType == DUI_MSGTYPE_ITEMACTIVATE && m_rosterList->GetItemIndex(msg.pSender) != -1)
	{
        CDuiString* l_departmentOrRosterNumber = reinterpret_cast<CDuiString*>(msg.pSender->GetTag());
        if (!m_rosterList->IsDepartment(std::wstring(l_departmentOrRosterNumber->GetData()))) {
            ShowChatDialog(*l_departmentOrRosterNumber);
            return;
        }
	}
    else if (msg.sType == DUI_MSGTYPE_ITEMACTIVATE && m_recentRosterList->GetItemIndex(msg.pSender) != -1) {
        CDuiString* l_rosterNumber = reinterpret_cast<CDuiString*>(msg.pSender->GetTag());
        ShowChatDialog(*l_rosterNumber);
        return;

    }
    else if (_tcsicmp(msg.sType, _T("itemclick")) == 0 && m_rosterList->GetItemIndex(msg.pSender) != -1)
	{
        // get the embedded department id
        CDuiString* l_departmentOrRosterNumber = reinterpret_cast<CDuiString*>(msg.pSender->GetTag());
        if (m_rosterList->IsDepartment(std::wstring(l_departmentOrRosterNumber->GetData()))) {
            std::wstring department = std::wstring(l_departmentOrRosterNumber->GetData());
            m_rosterList->RotateDepartmentAndEmployee(department);
            return;
        }
    }
    else if (msg.sType == DUI_MSGTYPE_MENU) {
        POINT pt = { msg.ptMouse.x, msg.ptMouse.y };
        auto roster_context_menu = new RosterContextMenu(*this, _T("xml/RosterContextMenu.xml"),
            pt, _T(""), this);
        roster_context_menu->ShowWindow(true, true);
        return;	
    }
    else if (msg.sType == _T("selectchanged")) {
        CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabs")));
        if (msg.pSender->GetName() == kRosterListOptionName) {
            if (pTabControl && pTabControl->GetCurSel() != 0) {
                pTabControl->SelectItem(0);
            }
        }
        if (msg.pSender->GetName() == kRecentRosterListOptionName) {
            if (pTabControl && pTabControl->GetCurSel() != 1) {
                pTabControl->SelectItem(1);
            }
        }
    }
}
void MainWindow::ChatDialogClosed(ChatWindow* p_chatDialog) {
    for (auto l_iterator = m_chatDialogs.begin(); l_iterator != m_chatDialogs.end(); l_iterator++) {
        if (l_iterator->second == p_chatDialog) {
            // update the recent contact
            m_recentRosterList->AddRecentRosterItem(RosterMgmt::Instance()->GetRosterItem(l_iterator->first));
            // LocalProfile::Instance()->AddRecentRosterItem(l_iterator->first);
            // 
            m_chatDialogs.erase(l_iterator);
            return;
        }
    }
    MLOG(logERROR) << __FUNCTION__ << " Inconsistency found.";
}
// Handle Customized message from background network thread, eg, Buddy list, chat message
LRESULT MainWindow::HandleCustomMessage(UINT p_messageId, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if(p_messageId == WM_INTERNAL_MESSAGE) {
        bHandled = true;
        //
        // Forward google proto buffer message to StateChart
        // 
        m_mainStateChart->ProcessEvent(std::shared_ptr<Event>(reinterpret_cast<Event*>(wParam)));
        return 0;
    }
    // 
    // Process the Roster Context Menu
    //
    if (p_messageId == WM_MENU_MESSAGE && (MenuItem)wParam == MenuItem::StartChat) {
        bHandled = true;
        auto rosterListItem = reinterpret_cast<CListContainerElementUI*>(m_rosterList->GetItemAt(m_rosterList->GetCurSel()));
        ShowChatDialog(*reinterpret_cast<CDuiString*>(rosterListItem->GetTag()));
    }
    if (p_messageId == WM_MENU_MESSAGE && (MenuItem)wParam == MenuItem::StartVideo) {
        bHandled = true;
        auto rosterListItem = reinterpret_cast<CListContainerElementUI*>(m_rosterList->GetItemAt(m_rosterList->GetCurSel()));
        auto rosterNumber = (*reinterpret_cast<CDuiString*>(rosterListItem->GetTag()));
#ifdef ENABLE_GOOGLE_WEBRTC
        return ShowVideoWindow(rosterNumber.GetData());
#endif
    }
	return 0;
}
void MainWindow::OnImPresence(im::Presence* p_message) {
    if (ToWstring(p_message->fromjid().id()) == RosterMgmt::Instance()->Myself().id) {
        MLOG(logDEBUG) << __FUNCTION__ << " received im.presence from other binded resource, discarded.";
        return;
    }
    RosterMgmt::Instance()->SetRosterItemStatus(ToWstring(p_message->fromjid().id()), p_message->show());
    m_rosterList->UpdateRosterItem(RosterMgmt::Instance()->GetRosterItem(ToWstring(p_message->fromjid().id())));
    m_recentRosterList->ChangeRecenRosterItemOnlineStauts(RosterMgmt::Instance()->GetRosterItem(ToWstring(p_message->fromjid().id())));
}

void MainWindow::OnChatMessage(std::shared_ptr<RichChatMessage> p_message) {
    std::wstring l_chatWithId;
    if (p_message->To() == RosterMgmt::Instance()->Myself().id) {
        l_chatWithId = p_message->From();
    }
    if (p_message->From() == RosterMgmt::Instance()->Myself().id) {
        l_chatWithId = p_message->To();
    }
	ASSERT(!l_chatWithId.empty());
	ChatWindow* l_chatDialog = this->ShowChatMessage(l_chatWithId, p_message);
}
#ifdef ENABLE_GOOGLE_WEBRTC
void MainWindow::OnP2pJingleMessage(p2p::JingleMessage* p_message){
    std::wstring l_fromId = ToWstring(p_message->fromjid().id());
    std::wstring l_toId = ToWstring(p_message->tojid().id());

    assert(l_toId == RosterMgmt::Instance()->Myself().id);

    if (m_videoWindow != nullptr && ToWstring(m_videoWindow->RosterNumber().id()) != l_fromId) {
        MLOG(logERROR) << __FUNCTION__ << "Unexpected Jingle Message received.";
        return;
    }
    ShowVideoWindow(l_fromId, false);
    m_videoWindow->ProcessJingleMessage(p_message);
    return;
}
#endif

ChatWindow* MainWindow::ShowChatMessage(std::wstring p_chatWithId, std::shared_ptr<RichChatMessage> p_message) {
	ChatWindow* l_chatDialog = nullptr;
	if (m_chatDialogs.find(p_chatWithId) == m_chatDialogs.end()) {
		l_chatDialog = CreateChatWindow(RosterMgmt::Instance()->GetRosterItem(p_chatWithId));
		m_chatDialogs[p_chatWithId] = l_chatDialog;
		::ShowWindow(l_chatDialog->GetHWND(), SW_MINIMIZE);
        LocalProfile::Instance()->AddChatMessage(p_message);
        // l_chatDialog->AppendMessage(p_message);
		NotifyUser(l_chatDialog->GetHWND());
	}
	else {
		l_chatDialog = m_chatDialogs[p_chatWithId];
        LocalProfile::Instance()->AddChatMessage(p_message);
        l_chatDialog->AppendMessage(p_message);
		NotifyUser(l_chatDialog->GetHWND());
	}

	return l_chatDialog;
}
ChatWindow* MainWindow::ShowChatDialog( const CDuiString& l_rosterId )
{
    RosterItem friend_info = RosterMgmt::Instance()->GetRosterItem(l_rosterId.GetData());
    if (m_chatDialogs.find(friend_info.id) == m_chatDialogs.end()) {
        m_chatDialogs[friend_info.id] = CreateChatWindow(friend_info);
    }
    if (SetForegroundWindow(*m_chatDialogs[friend_info.id]) == 0 ) {
         MLOG(logERROR) << " Can't SetForeGround the window. error code:" << GetLastError();
    }
    ::ShowWindow(*m_chatDialogs[friend_info.id], SW_RESTORE);
    return m_chatDialogs[friend_info.id];
}

ChatWindow* MainWindow::CreateChatWindow( RosterItem friend_info )
{
    auto pChatDialog = new ChatWindow(
        std::bind(&MainWindow::ChatDialogClosed, this, std::placeholders::_1),
        m_messageQueue, RosterMgmt::Instance()->Myself(), friend_info);
    pChatDialog->CenterWindow();

    // assume you contact someone when you are trying to chat with it
    m_recentRosterList->AddRecentRosterItem(friend_info);
    return pChatDialog;
}

void MainWindow::NotifyUser( HWND hwnd )
{
    StartFalshWindow(hwnd);
    // TODO: build msg.wav into resource ?
    ::PlaySound( (CPaintManagerUI::GetInstancePath() + _T("/Resources/media/msg.wav")).GetData(), NULL, SND_FILENAME | SND_ASYNC);
}

void MainWindow::StartFalshWindow( HWND hwnd )
{
    FLASHWINFO pf;
    pf.cbSize = sizeof(FLASHWINFO);
    pf.hwnd = hwnd;
    pf.dwFlags = FLASHW_TIMER|FLASHW_TRAY; // (or FLASHW_ALL to flash and if it is not minimized)
    pf.uCount = 0;
    pf.dwTimeout = 0;
    // ::ShowWindow(*m_chatDialogs[friend_info.id], SW_MINIMIZE);
    ::FlashWindowEx(&pf);
}


#ifdef ENABLE_GOOGLE_WEBRTC
void MainWindow::VideoWindowClosed(VideoWindow* p_videoWindow){
    m_videoWindow = nullptr;
}

LRESULT MainWindow::ShowVideoWindow(std::wstring rosterNumber, bool initialSide)
{
    if (m_videoWindow == nullptr) {
        core::Jid remotePeerId;
        remotePeerId.set_id(ToString(rosterNumber));
        m_videoWindow = new VideoWindow(initialSide, m_hWnd, remotePeerId, m_messageQueue, std::bind(&MainWindow::VideoWindowClosed, this, std::placeholders::_1));
        m_videoWindow->CenterWindow();
        ::ShowWindow(*m_videoWindow, SW_SHOW);
        return 0;
    }
    ::ShowWindow(*m_videoWindow, SW_RESTORE);
    // ::SetForegroundWindow(*m_videoWindow);
    return 0;
}
#endif

void MainWindow::OnTrayIconRButtonDown(CTrayIcon* pTrayIcon) {
    POINT point;
    GetCursorPos(&point);
    TrackPopupMenu(m_trayMenu, TPM_RIGHTBUTTON, point.x, point.y,
        0, m_hWnd, NULL);

}
LRESULT MainWindow::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled) {
    if (uMsg == WM_COMMAND) {
        if (LOWORD(wParam) == ID_SHOW_MAINWINDOW) {
            ::ShowWindow(m_hWnd, SW_SHOW);
            bHandled = true;
            return TRUE;
        }
        if (LOWORD(wParam) == ID_EXIT_MAINWINDOW) {
            m_mainStateChart->ProcessEvent(std::make_shared<Event>(Event(MessageType::Exit)));
            bHandled = true;
            return TRUE;
        }
        if (LOWORD(wParam) == ID_SIGNOFF) {
            m_mainStateChart->ProcessEvent(std::shared_ptr<Event>(new Event(MessageType::StartSignoff)));
            bHandled = true;
            return TRUE;
        }
        if (LOWORD(wParam) == ID_PRESENCE_ONLINE || LOWORD(wParam) == ID_TRAY_PRESENCE_ONLINE) {
            ChangePresenceStatus(false);
            bHandled = true;
            return TRUE;
        }
        if (LOWORD(wParam) == ID_PRESENCE_BUSY || LOWORD(wParam) == ID_TRAY_PRESENCE_BUSY) {
            ChangePresenceStatus(true);
            bHandled = true;
            return TRUE;
        }
        if (LOWORD(wParam) == ID_ABOUT_LIGHTYEAR) {
            ShowAboutWindow();
            bHandled = true;
            return TRUE;
        }
    }
    return WindowImplBase::MessageHandler(uMsg, wParam, lParam, bHandled);
}

void MainWindow::ChangePresenceStatus(bool isBusy) {
    if (isBusy) {
        CheckMenuItem(m_presenceStatusMenu, ID_PRESENCE_BUSY, MF_CHECKED);
        CheckMenuItem(m_trayMenu, ID_TRAY_PRESENCE_BUSY, MF_CHECKED);

        CheckMenuItem(m_presenceStatusMenu, ID_PRESENCE_ONLINE, MF_UNCHECKED);
        CheckMenuItem(m_trayMenu, ID_TRAY_PRESENCE_ONLINE, MF_UNCHECKED);
        m_presenceStatusBtn->SetBkImage(L"presence_busy.png");
        m_trayIcon->SetIcon(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)));
        m_mainStateChart->ProcessEvent(std::shared_ptr<Event>(new Event(MessageType::ChangeToAway)));
        return;
    }
    CheckMenuItem(m_presenceStatusMenu, ID_PRESENCE_BUSY, MF_UNCHECKED);
    CheckMenuItem(m_trayMenu, ID_TRAY_PRESENCE_BUSY, MF_UNCHECKED);

    CheckMenuItem(m_presenceStatusMenu, ID_PRESENCE_ONLINE, MF_CHECKED);
    CheckMenuItem(m_trayMenu, ID_TRAY_PRESENCE_ONLINE, MF_CHECKED);
    m_presenceStatusBtn->SetBkImage(L"presence_online.png");
    m_trayIcon->SetIcon(LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME)));
    m_mainStateChart->ProcessEvent(std::shared_ptr<Event>(new Event(MessageType::ChangeToAvailable)));
}

void MainWindow::CloseAll() {
    for (auto cw : m_chatDialogs) {
        cw.second->Close();
    }
    m_trayIcon->SetVisible(false);
    Close();
}
void MainWindow::StartHeartbeatTimer(){
    m_PaintManager.SetTimer(m_presenceStatusBtn, HEARTBEAT_TIMER_ID, 4000);

}
void MainWindow::StopHeartbeatTimer(){
    m_PaintManager.KillTimer(m_presenceStatusBtn, HEARTBEAT_TIMER_ID);
}
const int MainWindow::HEARTBEAT_TIMER_ID = 101; // 100 is the timer in signing in window

