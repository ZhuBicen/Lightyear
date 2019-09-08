#include "control/TrayMenu.hpp"
#include "main/MainWindow.hpp"
#include "base/StateChartBase.hpp"
#include "UiMessage.hpp"

TrayMenu::TrayMenu(HWND parent, LPCTSTR xmlPath, POINT pt, std::wstring rosterNumber, MainWindow* mw)
:LightyearMenu(parent, xmlPath, pt),
mw_(mw){

}
void TrayMenu::Notify(TNotifyUI& msg) {
    if (_tcsicmp(msg.sType, _T("itemclick")) == 0 && msg.pSender->GetName() == _T("ShowMainWindow")) {
        //Close(); -> delete this -> lead this.Notify not valid
        ::PostMessage(m_parent, WM_MENU_MESSAGE, (WPARAM)MenuItem::ShowMainWindow, 0);
        ::PostMessage(m_hWnd, WM_KILLFOCUS, 0, 0);
    }
    else if (_tcsicmp(msg.sType, _T("itemclick")) == 0 && msg.pSender->GetName() == _T("ExitMainWindow")) {
        ::PostMessage(m_parent, WM_MENU_MESSAGE, (WPARAM)MenuItem::ExitMainWindow, 0);
        ::PostMessage(m_hWnd, WM_KILLFOCUS, 0, 0);
    }
}