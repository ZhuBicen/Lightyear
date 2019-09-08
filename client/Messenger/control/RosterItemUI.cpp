#include "RosterItemUI.hpp"
#include "RosterMgmt.hpp"
#include "RosterLogoButton.hpp"

namespace DuiLib{
    static bool OnLogoButtonEvent(void* event) {
        if (((TEventUI*)event)->Type == UIEVENT_DBLCLICK) {
            CControlUI* pButton = ((TEventUI*)event)->pSender;
            if (pButton != NULL) {
                CListContainerElementUI* pListElement = (CListContainerElementUI*)(pButton->GetTag());
                if (pListElement != NULL) pListElement->DoEvent(*(TEventUI*)event);
            }
        }
        return true;
    }
    const TCHAR* const kLogoButtonControlName = _T("logo");
    const TCHAR* const kLogoContainerControlName = _T("logo_container");
    const TCHAR* const kNameControlName = _T("name");
    const TCHAR* const kPositionControlName = _T("position");

CControlUI* CRosterItemUI::CreateControl(LPCTSTR pstrClass) {
    if (_tcsicmp(pstrClass, _T("RosterLogoButton")) == 0) {
        return new CRosterLogoButton(m_pManager);
    }
    return NULL;
}
LPCTSTR CRosterItemUI::GetClass() const{
    return _T("RosterListItemUI");
}
LPVOID CRosterItemUI::GetInterface(LPCTSTR pstrName){
    if (_tcscmp(pstrName, _T("RosterListItemUI")) == 0) {
        return static_cast<CRosterItemUI*>(this);
    }
    return CListContainerElementUI::GetInterface(pstrName);
}

CRosterItemUI::CRosterItemUI(CPaintManagerUI& painterManager, const RosterItem& item):m_paintManager(painterManager) {
    CHorizontalLayoutUI* l_rosterElement = NULL;
    if (!m_dlgBuilder.GetMarkup()->IsValid()) {
        l_rosterElement = static_cast<CHorizontalLayoutUI*>(m_dlgBuilder.Create(_T("xml\\friend_list_item.xml"), (UINT)0, this, &m_paintManager));
    }
    else {
        l_rosterElement = static_cast<CHorizontalLayoutUI*>(m_dlgBuilder.Create(this, &m_paintManager));
    }
    if (l_rosterElement == NULL)
        return ;
    SetFixedHeight(50);
    l_rosterElement->SetFixedHeight(50);
    l_rosterElement->SetVisible(true);
    m_statusBtn = static_cast<CButtonUI*>(m_paintManager.FindSubControlByName(l_rosterElement, L"status"));
    m_logoBtn = static_cast<CRosterLogoButton*>(m_paintManager.FindSubControlByName(l_rosterElement, kLogoButtonControlName));

    m_logoBtn->SetTag((UINT_PTR)l_rosterElement);
    m_logoBtn->OnEvent += MakeDelegate(&OnLogoButtonEvent);

    m_nickName = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(l_rosterElement, kNameControlName));
    m_descriptionLabel = static_cast<CLabelUI*>(m_paintManager.FindSubControlByName(l_rosterElement, kPositionControlName));

    m_logoBtn->SetOnlineStatus(item.status);
    if (item.logo) {
        m_logoBtn->SetImage((*item.logo).c_str());
    }
    if (item.name) {
        CDuiString html_text;
        html_text += (*item.name).c_str();
        m_nickName->SetShowHtml(true);
        m_nickName->SetText(html_text);
    }
    if (item.position) {
        TCHAR szBuf[MAX_PATH] = { 0 };
        _stprintf_s(szBuf, MAX_PATH - 1, _T("<x 20><c #808080>%s</c>"), (*item.position).c_str());
        m_descriptionLabel->SetShowHtml(true);
        m_descriptionLabel->SetText(szBuf);
    }
    UpdateOnlineStatus(item.status);
    Add(l_rosterElement);
}
// precondition: ri.id 
void CRosterItemUI::UpdateOnlineStatus(im::Presence_Show status) {

    if (status == im::Presence_Show_ONLINE) {
        m_statusBtn->SetBkImage(L"presence_online.png");
    }
    else if (status == im::Presence_Show_AWAY) {
        m_statusBtn->SetBkImage(L"presence_away.png");
    }
    else if (status == im::Presence_Show_OFFLINE) {
        m_statusBtn->SetBkImage(L"presence_offline.png");
    }
}
}// namespace DuiLib