#pragma once
#include "control/LightyearMenu.hpp"
#include <string>
class MainWindow;
class RosterContextMenu: public LightyearMenu
{
public:
    RosterContextMenu(HWND parent, LPCTSTR xmlPath, POINT pt, std::wstring rosterNumber, MainWindow*);
    virtual void Notify(TNotifyUI& msg);

private:
    MainWindow* mw_;
    std::wstring roster_number_; // redundant
};