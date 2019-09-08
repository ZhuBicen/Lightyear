#undef NOMINMAX

#include "VideoWindow.hpp"
#include "windows.h"
#include <atlbase.h>
CComModule _Module;
#include <atlwin.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{

    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

    HINSTANCE hInstRich = ::LoadLibrary(_T("Riched20.dll"));

    ::CoInitialize(NULL);
    ::OleInitialize(NULL);

    _Module.Init(0, hInstance);

    HRESULT Hr = ::CoInitialize(NULL);
    if (FAILED(Hr)) return 0;

    auto vw = new VideoWindow(NULL);
    vw->CenterWindow();
    ::ShowWindow(*vw, SW_SHOW);

    CPaintManagerUI::MessageLoop();
    CPaintManagerUI::Term();
    return 0;
}