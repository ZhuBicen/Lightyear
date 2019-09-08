#pragma once
#include "..\DuiLib\UIlib.h"
using namespace DuiLib;
class VideoWindow: public WindowImplBase
{
public:
    VideoWindow(HWND);
    ~VideoWindow(void);

    LPCTSTR GetWindowClassName() const;

    virtual CControlUI* CreateControl(LPCTSTR pstrClass);

    virtual void OnFinalMessage(HWND hWnd);

    void Notify(TNotifyUI& msg);

    virtual void InitTheme() {
        //InitBackground();
        //InitTitleBar();
    }

    virtual CDuiString GetSkinFile() const;
    virtual CDuiString GetSkinFolder() const{
        return _T("../../LightyearClient/VideoWindowTest/skin");
    }

private:
    void InitBackground();
    void InitTitleBar();
};


class VideoControl: public DuiLib::CControlUI
{
public:
    VideoControl();
    virtual void DoPaint(HDC hDC, const RECT& rcPaint);
    virtual void SetFocus(){}
    virtual void DoEvent(TEventUI& event);

private:

};
