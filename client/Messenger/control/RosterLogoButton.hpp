#ifndef LIGHTYEAR_MESSENGER_LOGO_BUTTON_HPP
#define LIGHTYEAR_MESSENGER_LOGO_BUTTON_HPP
#pragma once
#include "../DuiLib/UiLib.h"
namespace DuiLib{
class CRosterLogoButton: public CLabelUI
{
public:
    CRosterLogoButton(CPaintManagerUI*);
    ~CRosterLogoButton(void);
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;
    void SetImage(LPCTSTR pStrImage){
        m_logoImage = pStrImage;
    };

    void SetOnlineStatus(im::Presence_Show status) {
        NeedUpdate();
        m_status = status;
    }

    void DoEvent(TEventUI& event);
    virtual void PaintStatusImage(HDC hDC);

private:
    CDuiString m_logoImage;
    im::Presence_Show m_status;
    CPaintManagerUI* m_paintManager;
};

}

#endif // !LIGHTYEAR_MESSENGER_LOGO_BUTTON_HPP