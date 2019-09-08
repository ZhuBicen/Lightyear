#pragma once
#include "window/LightyearWindowBase.hpp"
#include <boost/optional.hpp>
class ScreenshotWindow: public LightyearWinodwBase
{
public:
    ScreenshotWindow(HWND);
    ~ScreenshotWindow(void);

    static HBITMAP CopyScreen(void);

    LPCTSTR GetWindowClassName() const;

    virtual CControlUI* CreateControl(LPCTSTR pstrClass);

    virtual void OnFinalMessage(HWND hWnd);

    void Notify(TNotifyUI& msg);

    virtual void InitWindow();
    // implement LightyearWindowBase
    virtual CDuiString GetWindowText() const {
        return _T("Lightyear");
    }
    virtual CDuiString GetSkinFile() const;
private:
    HBITMAP m_screenBitmap;
    HWND m_chatDialog;
};


class ClientPainter: public DuiLib::CControlUI
{
public:
    ClientPainter(ScreenshotWindow* p_parentWindow, HWND p_chatDialog);
    virtual void DoPaint(HDC hDC, const RECT& rcPaint);
    virtual void SetFocus(){}
    virtual void DoEvent(TEventUI& event);

    void DrawSelected( RECT rect );

    void DrawBackground( RECT rc2);

private:
    HBITMAP m_bitmap;
    RECT m_screenRect;
    boost::optional<POINT> m_startPoint;
    boost::optional<POINT> m_endPoint;

    ScreenshotWindow* m_parentWindow;
    HWND m_chatDialog;

    int width, height;
public:
    HBITMAP GetSelectedBitmap(void);
};
