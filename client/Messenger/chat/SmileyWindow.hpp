#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP
#include "window/LightyearWindowBase.hpp"
#include <string>
#include <map>
#include <boost/tuple/tuple.hpp>

class ChatWindow;
class SmileyWindow : public LightyearWinodwBase
{
public:
	SmileyWindow(ChatWindow* chat_dialog, POINT ptMouse);

	LPCTSTR GetWindowClassName() const;

	virtual void OnFinalMessage(HWND hWnd);

	void Notify(TNotifyUI& msg);

	void InitWindow();

    // implement LightyearWindowBase
    virtual CDuiString GetWindowText() const {
        return _T("Lightyear");
    }
	virtual CDuiString GetSkinFile() const;
    
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    static void Init();

    static std::string SmileyWindow::EncodeHbitmap(HBITMAP p_hbitmap);
    static std::wstring SmileyWindow::FindSmiley(HBITMAP p_hbitmap);

    typedef std::map<std::wstring, boost::tuple<std::wstring, std::wstring, std::string> > SmileyMap;
	typedef SmileyMap::value_type SmileyPair;
private:

	POINT based_point_;
	ChatWindow* chat_dialog_;
public:
    static  SmileyMap s_smileyCollection;
};

#endif // COLORPICKER_HPP
