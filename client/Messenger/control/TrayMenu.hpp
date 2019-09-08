#pragma once
#include "control/LightyearMenu.hpp"
#include <string>
class MainWindow;
class TrayMenu: public LightyearMenu
{
public:
    TrayMenu(HWND parent, LPCTSTR xmlPath, POINT pt, std::wstring rosterNumber, MainWindow*);
    virtual void Notify(TNotifyUI& msg);

private:
    MainWindow* mw_;
};