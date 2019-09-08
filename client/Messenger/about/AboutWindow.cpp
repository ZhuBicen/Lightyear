#include "Version.h"
#include <sstream>
#include <windows.h>
void ShowAboutWindow()
{
    std::wostringstream version;
    version << "Version: " << VERSION_STRING << std::endl
        << "Revision: " << VERSION_REVISION << std::endl
        << "Build Time: " << VERSION_BUILD_DATE_TIME << std::endl;
    ::MessageBox(NULL, version.str().c_str(), L"关于本软件", MB_OK);
}