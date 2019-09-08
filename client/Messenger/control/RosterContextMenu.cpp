#include "control/RosterContextMenu.hpp"
#include "main/MainWindow.hpp"
#include "base/StateChartBase.hpp"
#include "UiMessage.hpp"

RosterContextMenu::RosterContextMenu(HWND parent, LPCTSTR xmlPath, POINT pt, std::wstring rosterNumber, MainWindow* mw)
:LightyearMenu(parent, xmlPath, pt),
roster_number_(rosterNumber),
mw_(mw){

}
void RosterContextMenu::Notify(TNotifyUI& msg) {
    if (_tcsicmp(msg.sType, _T("itemclick")) == 0 && msg.pSender->GetName() == _T("StartChatBtn")) {
        //Close(); -> delete this -> lead this.Notify not valid
        ::PostMessage(m_parent, WM_MENU_MESSAGE, (WPARAM)MenuItem::StartChat, 0);
        ::PostMessage(m_hWnd, WM_KILLFOCUS, 0, 0);
    }
    else if (_tcsicmp(msg.sType, _T("itemclick")) == 0 && msg.pSender->GetName() == _T("StartVideoBtn")) {
        ::PostMessage(m_parent, WM_MENU_MESSAGE, (WPARAM)MenuItem::StartVideo, 0);
        ::PostMessage(m_hWnd, WM_KILLFOCUS, 0, 0);
    }
}