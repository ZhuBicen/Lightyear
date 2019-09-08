#include "proto/src/im.pb.h"
#ifdef NOMINMAX
#undef NOMINMAX
#endif
#include "control/RosterLogoButton.hpp"

namespace DuiLib {
CRosterLogoButton::CRosterLogoButton(CPaintManagerUI* p_paintManager): m_status(im::Presence_Show_OFFLINE),
    m_paintManager(p_paintManager) {
}


CRosterLogoButton::~CRosterLogoButton(void) {
}

LPCTSTR CRosterLogoButton::GetClass() const {
    return _T("RosterLogoButtonUI");
}

LPVOID CRosterLogoButton::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, _T("RosterLogoButtonUI")) == 0 ) {
        return static_cast<CRosterLogoButton*>(this);
    }
    return CLabelUI::GetInterface(pstrName);
}

UINT CRosterLogoButton::GetControlFlags() const
{
    return (IsKeyboardEnabled() ? UIFLAG_TABSTOP : 0) | (IsEnabled() ? UIFLAG_SETCURSOR : 0);
}

void CRosterLogoButton::DoEvent(TEventUI& event) {

}
void CRosterLogoButton::PaintStatusImage(HDC hDC) {
    // http://blog.csdn.net/wangchyz/article/details/6795419
    // Mask似乎不起效？
    //DrawImage(hDC, _T("test.bmp")/*m_logoImage*/, _T("mask='#008AFF' fade='100'"));
    if (m_status != im::Presence_Show_OFFLINE) {
        DrawImage(hDC, m_logoImage);
        return;    
    }
    // 对于不在线的联系人，使用fade属性指定透明度
    DrawImage(hDC, m_logoImage, _T("fade='80'"));
}


}