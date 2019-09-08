#ifndef CHATDIALOG_HPP
#define CHATDIALOG_HPP
#include "window/LightyearWindowBase.hpp"
#include "chat/ChatMessage.hpp"
#include "RosterMgmt.hpp"
#include <functional>
#include <memory>

class MessageQueue;
class CefBrowserWrapper;

class ChatWindow : public LightyearWinodwBase
{
public:
	ChatWindow(std::function<void (ChatWindow*)>, MessageQueue&, const RosterItem& myselft_info, const RosterItem& friend_info);
	~ChatWindow();

public:

	LPCTSTR GetWindowClassName() const;	

	virtual void OnFinalMessage(HWND hWnd);

	virtual void InitWindow();

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

    // implement LightyearWindowBase
    virtual CDuiString GetWindowText() const;
	virtual CDuiString GetSkinFile() const;

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    virtual void UpdateTheme();
protected:	

	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

private:
	void FontStyleChanged();
    std::shared_ptr<RichChatMessage> GetUserInputText();
    
    // To notify user when networking error, peer is not online etc.
    void AppendNotification(const CDuiString&);

    void SendChatMessage();
public:
    void ReceiveMessage(const std::shared_ptr<RichChatMessage> p_chatMessage);
    void InsertEmotion(std::wstring p_emotionImage);
    void AppendMessage(std::shared_ptr<RichChatMessage> p_chatMessage);
private:	
	CDuiString bgimage_;
	DWORD bkcolor_;
	RosterItem myselft_;
	RosterItem m_ChattingWith;

    MessageQueue& message_queue_;
    std::function<void (ChatWindow*)> final_callback_;

private:
    CefBrowserWrapper* m_messageView;
    CRichEditUI* m_messageInput;
    CButtonUI* m_sendMessageButton;
    CProgressUI* m_loadingStatus;
    bool m_first;
};

#endif // CHARTDIALOG_HPP
