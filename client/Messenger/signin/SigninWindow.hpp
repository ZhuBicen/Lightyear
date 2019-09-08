#ifndef LIGHTYEAR_MESSENGER_SIGNIN_WINDOW_HPP
#define LIGHTYEAR_MESSENGER_SIGNIN_WINDOW_HPP
#include <memory> // for unique_ptr
#include "proto/src/core.pb.h"
#include "resource.h"
#include "window/LightyearWindowBase.hpp"

class MessageQueue;
class MainStateChart;

class SigninWindow : public LightyearWinodwBase
{
public:

    friend class MainStateChart;

    SigninWindow(MessageQueue& p_messageQueue, MainStateChart& p_mainStateChart);
    ~SigninWindow();
    // implement LightyearWindowBase
    virtual CDuiString GetWindowText() const {
        return _T("");
    }
	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMainFrame");  }
	virtual CDuiString GetSkinFile() const          {   return _T("xml\\SigninWindow.xml");  }
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
    virtual void InitWindow();
    virtual bool HasMaxButton() const { return false; }

    void Signin();
protected:
	void Notify(TNotifyUI& msg);



private:
	MessageQueue& m_messageQueue;
    
    CLabelUI* m_signinButton;
    CLabelUI* m_infoLabel; 
    CEditUI*  m_accountEdit;
    CEditUI* m_pwEdit;
    UINT m_timerId;

    std::unique_ptr<MainStateChart> m_mainStateChart;
};
#endif
