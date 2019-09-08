#ifndef GALLERY_08_CEFBROWSER_HPP
#define GALLERY_08_CEFBROWSER_HPP
#include <string>
#include <../DuiLib/UILib.h>
using namespace DuiLib;
#undef GetNextSibling
#undef GetFirstChild
#include "include/cef_base.h"
#include "include/cef_browser.h"

class CefBrowserWrapper : public CControlUI
{
public:
    CefBrowserWrapper(HWND parent, std::wstring id) :parent_window_(parent), chat_with_id_(id) {}
    virtual void SetPos(RECT rc);
    void AppendMessage(const std::wstring& content);
    void AppendNotification(const std::wstring& notice);
    void LoadUrl(const std::wstring& url);
private:
    HWND parent_window_;
    std::wstring chat_with_id_;
};

#endif