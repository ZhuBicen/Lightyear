#include "CefBrowserWrapper.hpp"
#include <string>
#undef GetNextSibling
#include "CefClientImpl.hpp"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "utility.hpp"

CefClientImpl* g_sh = nullptr;
void CefBrowserWrapper::SetPos(RECT rc) {

    __super::SetPos(rc);
    if (g_sh == nullptr) {
        g_sh = new CefClientImpl;
    }
    if (g_sh->GetBrowser(parent_window_) != nullptr) {
        HWND brower_wnd = g_sh->GetBrowser(parent_window_)->GetHost()->GetWindowHandle();
        ::SetWindowPos(brower_wnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
        return;
    }

    CefWindowInfo window_info;

    window_info.SetAsChild(parent_window_, rc);

    // SimpleHandler implements browser-level callbacks.
    CefRefPtr<CefClientImpl> handler(g_sh);

    // Specify CEF browser settings here.
    CefBrowserSettings browser_settings;
    browser_settings.file_access_from_file_urls = STATE_ENABLED;
    browser_settings.web_security = STATE_DISABLED;
    browser_settings.plugins = STATE_DISABLED;
    // image_shrink_standalone_to_fit
    //browser_settings.local_storage = STATE_ENABLED;
    // to load a blank url "about:blank"
    std::string url = "lightyear://messageHistory/" + ToString(chat_with_id_);
    // Create the first browser window.
    CefBrowserHost::CreateBrowser(window_info, handler.get(), url, browser_settings, NULL);
}

void CefBrowserWrapper::AppendMessage( const std::wstring& content) {
    std::wstring js =
        L"var msgs = document.getElementById(\"messages\");"
        L"var msg = document.createElement(\"div\");"
        L"msg.innerHTML=\"";
    js += content;
    js += L"\";";
    js += L"msgs.appendChild(msg);";
    js += L"window.scrollTo(0,document.body.scrollHeight);";
    // TODO: this can be crashed, due to the NULL browser
    g_sh->GetBrowser(parent_window_)->GetMainFrame()->ExecuteJavaScript(
        js,
        __FILE__,
        __LINE__);
}
void CefBrowserWrapper::AppendNotification(const std::wstring& notice) {
    std::wstring js =
        L"var msgs = document.getElementById(\"messages\");"
        L"var msg = document.createElement(\"div\");"
        L"msg.innerHTML=\"<p class='notice'>";
    js += notice;
    js += L"</p>\";";
    js += L"msgs.appendChild(msg);";
    js += L"window.scrollTo(0,document.body.scrollHeight);";
    // TODO: this can be crashed, due to the NULL browser
    g_sh->GetBrowser(parent_window_)->GetMainFrame()->ExecuteJavaScript(
        js,
        __FILE__,
        __LINE__);
}
void CefBrowserWrapper::LoadUrl(const std::wstring& url) {
    g_sh->GetBrowser(parent_window_)->GetMainFrame()->LoadURL(url);
}