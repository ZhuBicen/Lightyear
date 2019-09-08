#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP
#include <map>
#include <memory>
#include "window/LightyearWindowBase.hpp"
#include "control/RosterList.hpp"
#include "control/TrayIcon.hpp"
class MessageQueue;
class ChatWindow;
namespace DuiLib {
    class CRecentRosterList;
}

#ifdef ENABLE_GOOGLE_WEBRTC
class VideoWindow;
#endif

class MainStateChart;
class RichChatMessage;

namespace im {
	class RosterResp;
	class Presence;
	class ChatMessage;
}
namespace p2p {
	class JingleMessage;
}
class MainWindow : public LightyearWinodwBase, public ITrayIconListener
{
public:

	MainWindow(MessageQueue&, MainStateChart*);


    ~MainWindow();

	LPCTSTR GetWindowClassName() const;	
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
    virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);

    // implement LightyearWindowBase
    virtual CDuiString GetWindowText() const {
        return _T("");
    }
    virtual bool HasSkinButton() const { return false; }
	virtual CDuiString GetSkinFile() const ;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    DWORD GetBkColor();
    void SetBkColor(DWORD dwBackColor);
    virtual bool HasBackgroundImage() const{ return true; }
    // To handle customized WM_USER message, proto message was boxed.
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#ifdef ENABLE_GOOGLE_WEBRTC
    LRESULT ShowVideoWindow(std::wstring rosterId, bool initialSide = true);
#endif
    // Implement trayIconListener
    virtual void OnTrayIconLButtonDown(CTrayIcon* pTrayIcon) {}
    virtual void OnTrayIconLButtonUp(CTrayIcon* pTrayIcon) {
        ::ShowWindow(m_hWnd, SW_SHOW);
        ::SetForegroundWindow(m_hWnd);
    }
    virtual void OnTrayIconLButtonDblClk(CTrayIcon* pTrayIcon) {
        ::ShowWindow(m_hWnd, SW_SHOW);
        ::SetForegroundWindow(m_hWnd);
    }

    virtual void OnTrayIconRButtonDown(CTrayIcon* pTrayIcon);
    virtual void OnTrayIconRButtonUp(CTrayIcon* pTrayIcon) {}

public:
    ChatWindow* ShowChatDialog(const CDuiString& l_rosterId);
private:	

	void Notify(TNotifyUI& msg);
    void NotifyUser( HWND hwnd );
    void StartFalshWindow( HWND hwnd );
    ChatWindow* CreateChatWindow( RosterItem friend_info );
    void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

	void UpdateFriendsList();
    // callback when chatting dialog closed 
    void ChatDialogClosed(ChatWindow* p_chatDialog);
#ifdef ENABLE_GOOGLE_WEBRTC
    // callback when video window closed
    void VideoWindowClosed(VideoWindow* p_videoWindow);
#endif


private: // proto message
	MessageQueue& m_messageQueue;

private: // To keep all the opened chatting dialogs.
    ChatWindow* ShowChatMessage(std::wstring p_chatWithId, std::shared_ptr<RichChatMessage> p_message);
    std::map<std::wstring, ChatWindow*> m_chatDialogs;

#ifdef ENABLE_GOOGLE_WEBRTC
    VideoWindow* m_videoWindow;
#endif

public: // Process incoming proto buffer message
    void OnImPresence(im::Presence*);
    void OnChatMessage(std::shared_ptr<RichChatMessage>);
#ifdef ENABLE_GOOGLE_WEBRTC
    void OnP2pJingleMessage(p2p::JingleMessage* p_message);
#endif
    void ChangePresenceStatus(bool isBusy);
    // Close main window and all chat windows.
    void CloseAll();

    void StartHeartbeatTimer();
    void StopHeartbeatTimer();


private:
    MainStateChart* m_mainStateChart;
    CRosterList* m_rosterList;
    CRecentRosterList* m_recentRosterList;
    CButtonUI*  m_presenceStatusBtn;
    HMENU m_trayMenu;
    HMENU m_presenceStatusMenu;
    CTrayIcon* m_trayIcon;
public:
    static const int HEARTBEAT_TIMER_ID;

};

#endif // MAINFRAME_HPP
