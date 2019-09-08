#include <atlbase.h>
#include <atlwin.h>
#include "Log.hpp"
#include "resource.h"
#include "sc/StateChart.hpp"
#include "window/LightyearWindowBase.hpp" // For Duilib
#include "video/WebRtc.hpp"
#include "Utility.hpp"
#undef GetNextSibling
#undef GetFirstChild
#include "cef/CefAppImpl.hpp"

//CComModule _Module;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{

    // Provide CEF with command-line arguments.
    CefMainArgs main_args(hInstance);

    // SimpleApp implements application-level callbacks. It will create the first
    // browser instance in OnContextInitialized() after CEF has initialized.
    CefRefPtr<CefAppImpl> app(new CefAppImpl);

    // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
    // that share the same executable. This function checks the command-line and,
    // if this is a sub-process, executes the appropriate logic.
    int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
    if (exit_code >= 0) {
        // The sub-process has completed so return here.
        return exit_code;
    }
    // Specify CEF global settings here.
    CefSettings settings;
    // settings.background_color = CefColorSetARGB(10, 0xFF, 0xFF, 0xEE);
    settings.multi_threaded_message_loop = true;
    CefString(&settings.resources_dir_path).FromWString(GetBinaryPathW() + L"/Resources");
    CefString(&settings.locales_dir_path).FromWString(GetBinaryPathW() + L"/Resources/locales");
    // Initialize CEF.
    CefInitialize(main_args, settings, app.get(), NULL);

    // Initialize WebRTC
    // InitWebRtc();
	MLOG(logINFO)<<"WinMain is started";

    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	HINSTANCE hInstRich = ::LoadLibrary(_T("Riched20.dll"));

    MainStateChart l_SC;

	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();

	::FreeLibrary(hInstRich);
    // CleanWebRtc();
	MLOG(logINFO) << "WinMain exit";
	return 0;
}
