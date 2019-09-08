#pragma once
#include "../DuiLib/UiLib.h"
struct RosterItem;

namespace im{
    enum Presence_Show;
}
namespace DuiLib {

class CRosterLogoButton;

class CRosterItemUI : public CListContainerElementUI, IDialogBuilderCallback {

public:
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    CRosterItemUI(CPaintManagerUI&, const RosterItem& ri);
    void UpdateOnlineStatus(im::Presence_Show status);
    CControlUI* CreateControl(LPCTSTR pstrClass);
private:
    CPaintManagerUI& m_paintManager;
    CDialogBuilder m_dlgBuilder;

    CButtonUI* m_statusBtn;
    CRosterLogoButton* m_logoBtn;
    CLabelUI* m_nickName;
    CLabelUI* m_descriptionLabel;

};
} // namespace DuiLib