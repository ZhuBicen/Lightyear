#include "chat/ChatWindow.hpp"
#include "chat/ScreenshotWindow.hpp"
#include "chat/SmileyWindow.hpp"
#include "chat/ChatMessage.hpp"

#include "control/RosterLogoButton.hpp"
#include "cef/CefBrowserWrapper.hpp"
#include "cef/CefClientImpl.hpp"

#include "include/cef_task.h"
#include "include/base/cef_bind.h"
#include "include/wrapper/cef_closure_task.h"

#include "LocalProfile.hpp"
#include "MessageBuilder.hpp"
#include "RosterMgmt.hpp"
#include "MessageQueue.hpp"
#include "resource.h"
#include "ImageDataObject.hpp"
#include "UiMessage.hpp"
#include "proto/src/p2p.pb.h"
#include "Log.hpp"

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

const TCHAR* const kBackgroundControlName = _T("bg");

const TCHAR* const kLogoButtonControlName = _T("logo");
const TCHAR* const kNickNameControlName = _T("nickname");
const TCHAR* const kPositionControlName = _T("position");

const TCHAR* const kFontButtonControlName = _T("fontbtn");
const TCHAR* const kFontbarControlName = _T("fontbar");
const TCHAR* const kFontTypeControlName = _T("font_type");
const TCHAR* const kFontSizeControlName = _T("font_size");
const TCHAR* const kBoldButtonControlName = _T("boldbtn");
const TCHAR* const kItalicButtonControlName = _T("italicbtn");
const TCHAR* const KUnderlineButtonControlName = _T("underlinebtn");
const TCHAR* const kColorButtonControlName = _T("colorbtn");

const TCHAR* const kInputRichEditControlName = _T("input_richedit");
const TCHAR* const kViewMessageHisotry = _T("messageHistoryView");

const TCHAR* const kEmotionButtonControlName = _T("emotionbtn");

const TCHAR* const kSendButtonControlName = _T("sendbtn");

const int kEmotionRefreshTimerId = 1001;
const int kEmotionRefreshInterval = 150;

extern CefClientImpl* g_sh;

ChatWindow::ChatWindow(std::function<void(ChatWindow*)> p_callback, MessageQueue& messageQueue, const RosterItem& myselft_info, const RosterItem& friend_info)
: final_callback_(p_callback)
, message_queue_(messageQueue)
, myselft_(myselft_info)
, m_ChattingWith(friend_info)
, m_messageView(nullptr)
, m_first(true)
{
    Create(NULL, GetWindowText(), UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
}

ChatWindow::~ChatWindow()
{
	return;
}

LPCTSTR ChatWindow::GetWindowClassName() const
{
	return _T("ChatDialog");
}

CControlUI* ChatWindow::CreateControl(LPCTSTR pstrClass)
{
    if (_tcsicmp(pstrClass, _T("RosterLogoButton")) == 0) {
        return new CRosterLogoButton(&m_PaintManager);
    }
    if (_tcsicmp(pstrClass, _T("CefBrowserWrapper")) == 0) {
        return new CefBrowserWrapper(m_hWnd, m_ChattingWith.id);
    }
    return NULL;
}

void ChatWindow::OnFinalMessage(HWND hWnd)
{
    final_callback_(this);
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}
CDuiString ChatWindow::GetSkinFile() const
{
	return _T("xml\\ChatWindow.xml");
}

CDuiString ChatWindow::GetWindowText() const {
    std::wostringstream l_wos;
    l_wos << _T("与 ") << (m_ChattingWith.name ? *m_ChattingWith.name: m_ChattingWith.id )  << _T(" 聊天中...");
    return l_wos.str().c_str();
}

void ChatWindow::UpdateTheme(){
    LightyearWinodwBase::UpdateTheme();
}

LRESULT ChatWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    g_sh->GetBrowser(m_hWnd)->GetHost()->CloseBrowser(true);
	bHandled = FALSE;
	return 0;
}

LRESULT ChatWindow::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    FLASHWINFO pf;
    pf.cbSize = sizeof(FLASHWINFO);
    pf.hwnd = GetHWND();
    pf.dwFlags = FLASHW_STOP;
    pf.uCount = 0;
    pf.dwTimeout = 0;
    ::FlashWindowEx(&pf);
	bHandled = TRUE;
	return 0;
}

LRESULT ChatWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ChatWindow::ResponseDefaultKeyEvent(WPARAM wParam)
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

void ChatWindow::OnExit(TNotifyUI& msg)
{
	Close();
}

void ChatWindow::InitWindow(){
    // TODO: 设置当前会话的用户的LOGO
    SetIcon(IDR_MAINFRAME);
    m_messageView  = static_cast<CefBrowserWrapper*>(m_PaintManager.FindControl(kViewMessageHisotry));
    m_messageInput = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));

    //for (auto l_chatMessage: LocalProfile::Instance()->QueryChatMessageById(m_ChattingWith.id)) {
    //    AppendMessage(l_chatMessage->From(), l_chatMessage);
    //}
}

void ChatWindow::OnPrepare(TNotifyUI& msg)
{
	CButtonUI* log_button = static_cast<CButtonUI*>(m_PaintManager.FindControl(kLogoButtonControlName));
    if (m_ChattingWith.logo) {
		log_button->SetNormalImage((*m_ChattingWith.logo).c_str());
	}
	CControlUI* nick_name = m_PaintManager.FindControl(kNickNameControlName);
    if (m_ChattingWith.name) {
        nick_name->SetText((*m_ChattingWith.name).c_str());
    }
	CControlUI* position = m_PaintManager.FindControl(kPositionControlName);
    if (m_ChattingWith.position) {
        position->SetText((*m_ChattingWith.position).c_str());
    }
    m_sendMessageButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(kSendButtonControlName));
    m_sendMessageButton->SetEnabled(false);
    m_loadingStatus = static_cast<CProgressUI*>(m_PaintManager.FindControl(L"loadingStatus"));
}


void ChatWindow::OnTimer(TNotifyUI& msg) {
    if (msg.wParam == WM_TIMER_CEF_LOADING) {
        MLOG(logDEBUG) << __FUNCTION__;
        
        if (m_loadingStatus->GetValue() + 10 < m_loadingStatus->GetMaxValue()) {
            m_loadingStatus->SetValue(m_loadingStatus->GetValue() + 10);
        }
        else {
            m_loadingStatus->SetValue(0);
        }
    }
}


std::shared_ptr<RichChatMessage>
ChatWindow::GetUserInputText() {
    CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
    pRichEdit->SetFocus();
    std::wstring l_textMessage = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength());
    if( l_textMessage.empty() ){
        return nullptr;
    }

    auto l_reOle = pRichEdit->GetRichEditOle();

    std::vector< std::pair<int, HBITMAP> > l_bitmaps;

    MLOG(logINFO) << __FUNCTION__ << "Inserted object count = " << l_reOle->GetObjectCount();
    for (auto i = 0; i < l_reOle->GetObjectCount(); i++)
    {
        REOBJECT l_reo;
        ZeroMemory(&l_reo, sizeof(REOBJECT));
        l_reo.cbStruct = sizeof(REOBJECT);
        HRESULT l_ret = l_reOle->GetObject(i, &l_reo, REO_GETOBJ_ALL_INTERFACES);
        if (l_ret != S_OK) {
            MLOG(logERROR) << __FUNCTION__ << "ERROR value = " << l_ret;
            if (l_ret == E_INVALIDARG ) {
                MLOG(logERROR) << __FUNCTION__ << " Invalid arguments.";
            } 
            continue;
        }
        auto l_oleObject = l_reo.poleobj;
        IDataObject* l_dataObject;
        l_oleObject->QueryInterface(IID_IDataObject, (void **)&l_dataObject);
        STGMEDIUM stgm;
        stgm.tymed = TYMED_GDI;
        FORMATETC fm;
        fm.cfFormat = CF_BITMAP;				// Clipboard format = CF_BITMAP
        fm.ptd = NULL;							// Target Device = Screen
        fm.dwAspect = DVASPECT_CONTENT;			// Level of detail = Full content
        fm.lindex = -1;							// Index = Not applicable
        fm.tymed = TYMED_GDI;					// Storage medium = HBITMAP handle

        if (S_OK != l_dataObject->GetData(&fm, &stgm)){
            MLOG(logERROR) << __FUNCTION__ << " ERROR, when get the HBITMAP";
            continue;
        }

        l_bitmaps.push_back(std::make_pair(l_reo.cp, stgm.hBitmap));
    }
    std::shared_ptr<RichChatMessage> l_chatMessage = 
        std::make_shared<RichChatMessage>(l_textMessage, l_bitmaps);
    l_chatMessage->SetFrom(RosterMgmt::Instance()->Myself().id);
    l_chatMessage->SetTo(m_ChattingWith.id);

    pRichEdit->SetText(_T(""));
    return l_chatMessage;
}

void ChatWindow::SendChatMessage() {
    CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
    MLOG(logINFO) << __FUNCTION__ << "Inserted object count = " << pRichEdit->GetRichEditOle()->GetObjectCount();
    if (!RosterMgmt::Instance()->IsOnline(m_ChattingWith.id)) {
        AppendNotification(_T("当前用户目前不在线，无法发送消息！"));
        return;
    }
    auto l_chatMessage = GetUserInputText();
    if (l_chatMessage == nullptr) {
        return;
    }
    AppendMessage(l_chatMessage);
    // save to the message history
    LocalProfile::Instance()->AddChatMessage(l_chatMessage);
    message_queue_.PutMessage(MessageBuilder::BuildChatMessage(l_chatMessage));
}
void ChatWindow::ReceiveMessage(std::shared_ptr<RichChatMessage> p_chatMessage){
    LocalProfile::Instance()->AddChatMessage(p_chatMessage);
    AppendMessage(p_chatMessage);
}

void ChatWindow::AppendNotification(const CDuiString& notice) {
     m_messageView->AppendNotification(notice.GetData());
}
void ChatWindow::AppendMessage(const std::shared_ptr<RichChatMessage> p_chatMessage) {
    m_messageView->AppendMessage(p_chatMessage->AsHtml());
}

void ChatWindow::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kEmotionButtonControlName) == 0)
		{
			POINT pt = {0};
			CDuiRect rcEmotionBtn = msg.pSender->GetPos();
			CDuiRect rcWindow;
			GetWindowRect(m_hWnd, &rcWindow);

			pt.y = rcWindow.top + rcEmotionBtn.top;
			pt.x = rcWindow.left + rcEmotionBtn.left;
			//emotion_list_window_.SelectEmotion(pt);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSendButtonControlName) == 0)
        {
            SendChatMessage();
		}
        else if (_tcsicmp(msg.pSender->GetName(), _T("screenshotsbtn")) == 0) {
            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
            new ScreenshotWindow(m_hWnd);
            // CImageDataObject::InsertImage(m_messageInput->GetRichEditOle(), );
            return;	
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("sendfilebtn")) == 0) {
            // To create P2P connection with remote peer.
            // if (p2p_ == nullptr) {
			// 	core::Jid l_peer_id;
			// 	l_peer_id.set_id(ConvertUtf16ToUtf8(friend_.id));
            //     p2p_ = new talk_base::RefCountedObject<Conductor>(l_peer_id, boost::ref(message_queue_), nullptr);
            // }
            // p2p_->Connect();
		}
        else if (msg.pSender->GetName() == _T("VideoButton")) {
        }
		else {
            __super::OnClick(msg);
        }
	}
    else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
    {
        if (_tcsicmp(msg.pSender->GetName(), kInputRichEditControlName) == 0)
        {
            SendChatMessage();
        }
    }
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
        if (_tcsicmp(msg.pSender->GetName(), _T("chatbox_Chat")) == 0 ) 
        {
            CVerticalLayoutUI* pTabControl = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("inputGroup")));
            pTabControl->SetVisible(true);
            m_messageView->LoadUrl(L"lightyear://messageHistory/" + m_ChattingWith.id);
        }
        if (_tcsicmp(msg.pSender->GetName(), _T("chatbox_History")) == 0 ) 
        {
            CVerticalLayoutUI* pTabControl = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("inputGroup")));
            pTabControl->SetVisible(false);
            m_messageView->LoadUrl(L"http://" + LocalProfile::Instance()->ServerIP()+ L":8088/messages?id1=" + m_ChattingWith.id + L"&id2=" + myselft_.id);
        }
		if (_tcsicmp(msg.pSender->GetName(), _T("chatbox_EmotionButton")) == 0)
		{
            POINT pt = {0};
            CDuiRect rcColorBtn = msg.pSender->GetPos();
            CDuiRect rcWindow;
            GetWindowRect(m_hWnd, &rcWindow);

            pt.y = rcWindow.top + rcColorBtn.top;
            pt.x = rcWindow.left + rcColorBtn.left;// + static_cast<LONG>(rcColorBtn.right - rcColorBtn.left / 2);
            new SmileyWindow(this, pt);
		}
	}
}



LRESULT ChatWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
    if (uMsg == WM_SCREENSHOT) {
        HBITMAP l_bitmap = reinterpret_cast<HBITMAP>(wParam);
        CImageDataObject::InsertImage(m_messageInput->GetRichEditOle(), l_bitmap);
        ::ShowWindow(m_hWnd, SW_RESTORE);
    }
    if (uMsg == WM_CEF_LOADED) {
        MLOG(logDEBUG) << __FUNCTION__ << " Stop loading timer.";
        m_sendMessageButton->SetEnabled(true);
        m_loadingStatus->SetVisible(false);
        m_PaintManager.KillTimer(m_loadingStatus, WM_TIMER_CEF_LOADING);
    }
    if (uMsg == WM_CEF_LOADING) {
        MLOG(logDEBUG) << __FUNCTION__ << " Start loading timer.";
        m_loadingStatus->SetVisible(true);
        if (!m_PaintManager.SetTimer(m_loadingStatus, WM_TIMER_CEF_LOADING, 500)) {
            MLOG(logDEBUG) << __FUNCTION__ << " Fail to start the loading timer.";
        }
    }
	return 0;
}

void ChatWindow::FontStyleChanged()
{}

void ChatWindow::InsertEmotion(std::wstring p_emotionImage){
    CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
    IRichEditOle *RichOle = pRichEdit->GetRichEditOle();
    CImageDataObject::InsertImage(RichOle, p_emotionImage);
}

//void ChatWindow::ProcessJingleMessage(p2p::JingleMessage* p_message) {
    // if (p2p_ == nullptr) {
    //     core::Jid l_peer_id;
    //     l_peer_id.set_id(ConvertUtf16ToUtf8(friend_.id));
    //     p2p_ = new talk_base::RefCountedObject<Conductor>(l_peer_id, boost::ref(message_queue_), nullptr);
    // }
	// p2p_->OnMessageFromPeer(p_message->content());
//}
