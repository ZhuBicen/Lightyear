#pragma once
#include "window/LightyearWindowBase.hpp"
#include "Log.hpp"
class LightyearMenu : public LightyearWinodwBase
{
protected:
    virtual ~LightyearMenu(){};        // 私有化析构函数，这样此对象只能通过new来生成，而不能直接定义变量。就保证了delete this不会出错
    CDuiString  m_strXMLPath;
 
public:
    explicit LightyearMenu(HWND hWndParent, LPCTSTR pszXMLPath, POINT pt):
        m_strXMLPath(pszXMLPath), pt_(pt),
        m_parent(hWndParent) {
        POINT ptPos = pt_;
        Create(hWndParent, _T("MenuWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE | WS_EX_TOPMOST);
        // MLOG(logINFO) << "New created menu:" << std::hex << m_hWnd << ", parent=" << hWndParent;
        ::ClientToScreen(hWndParent, &ptPos);
        ::SetWindowPos(*this, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
    }
    virtual LPCTSTR    GetWindowClassName()const{ return _T("LightyearMenu"); }
    virtual CDuiString GetWindowText() const { return _T(""); }
    virtual CDuiString GetSkinFile() const         { return m_strXMLPath;     }
    virtual void       OnFinalMessage(HWND hWnd){ 
        // MLOG(logINFO) << "PaintManager[" << &m_PaintManager << "]" << __FUNCTION__ << ", HWND = " << std::hex << m_hWnd;
        WindowImplBase::OnFinalMessage(hWnd);
        delete this;
    }
 
    virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        Exit();
        return 0;
    }

    void Exit()
    {
        Close();
        ::DestroyWindow(m_hWnd); // should destroy the window explicitly, else OnFinalMessage not called.
    }

    virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
        // MLOG(logINFO) << __FUNCTION__ << std::hex << " HWND lost focus:" << wParam;
        return 0;
    }
private:
    POINT pt_;
protected:
    HWND m_parent;
};