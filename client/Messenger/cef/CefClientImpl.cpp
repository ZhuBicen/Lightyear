#include "CefClientImpl.hpp"

#include <sstream>
#include <string>
#include "UiMessage.hpp"

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace {

    CefClientImpl* g_instance = NULL;

}  // namespace
CefClientImpl::CefClientImpl()
: is_closing_(false) {
    DCHECK(!g_instance);
    g_instance = this;
}

CefClientImpl::~CefClientImpl() {
    g_instance = NULL;
}

CefClientImpl* CefClientImpl::GetInstance() {
    return g_instance;
}

void CefClientImpl::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    browsers_[::GetParent(browser->GetHost()->GetWindowHandle())] = browser;
    // Need to know what's the intention of this CefBrowser?
    //std::stringstream ss;
    //ss << "<html><body bgcolor=\"white\"><h1>Chatting with xxx</h1>Text:<pre>" << "XXXYY" <<
    //    "</pre></body></html>";
    //browser->GetMainFrame()->LoadString(ss.str(), "http://tests/gettext");
}

bool CefClientImpl::DoClose(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();
    is_closing_ = true;
    return false;
}

void CefClientImpl::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
    CEF_REQUIRE_UI_THREAD();
}

void CefClientImpl::OnLoadError(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    ErrorCode errorCode,
    const CefString& errorText,
    const CefString& failedUrl) {
    CEF_REQUIRE_UI_THREAD();


}
enum client_menu_ids {
    CLIENT_ID_SHOW_DEVTOOLS = MENU_ID_USER_FIRST,
    CLIENT_ID_CLOSE_DEVTOOLS,
    CLIENT_ID_TESTMENU_SUBMENU,
    CLIENT_ID_TESTMENU_CHECKITEM,
    CLIENT_ID_TESTMENU_RADIOITEM1,
    CLIENT_ID_TESTMENU_RADIOITEM2,
    CLIENT_ID_TESTMENU_RADIOITEM3,
};
void CefClientImpl::OnBeforeContextMenu(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    CefRefPtr<CefMenuModel> model) {
    CEF_REQUIRE_UI_THREAD();

    if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0) {
        // Add a separator if the menu already has items.
        if (model->GetCount() > 0)
            model->AddSeparator();

        // Add DevTools items to all context menus.
        model->AddItem(CLIENT_ID_SHOW_DEVTOOLS, "&Show DevTools");
        model->AddItem(CLIENT_ID_CLOSE_DEVTOOLS, "Close DevTools");
    }
}
void CefClientImpl::ShowDevTools(CefRefPtr<CefBrowser> browser) {
    CefWindowInfo windowInfo;
    CefBrowserSettings settings;

#if defined(OS_WIN)
    windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), "DevTools");
#endif

    browser->GetHost()->ShowDevTools(windowInfo, this, settings, CefPoint(0, 0));
}

void CefClientImpl::CloseDevTools(CefRefPtr<CefBrowser> browser) {
    browser->GetHost()->CloseDevTools();
}
bool CefClientImpl::OnContextMenuCommand(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefContextMenuParams> params,
    int command_id,
    EventFlags event_flags) {
    CEF_REQUIRE_UI_THREAD();

    switch (command_id) {
    case CLIENT_ID_SHOW_DEVTOOLS:
        ShowDevTools(browser);
        return true;
    case CLIENT_ID_CLOSE_DEVTOOLS:
        CloseDevTools(browser);
        return true;
    default:
        return true;
    }
}
void CefClientImpl::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) {
    CEF_REQUIRE_UI_THREAD();
    for (auto& wndBrowser : browsers_) {
        if (wndBrowser.second->IsSame(browser)) {
            HWND hwnd = wndBrowser.first;
            // POST message to DUILIB_UI_THREAD
            if (isLoading) {
                ::PostMessage(hwnd, WM_CEF_LOADING, 0, 0);
            }
            else {
                ::PostMessage(hwnd, WM_CEF_LOADED, 0, 0);
            }
        }
    }
}

void CefClientImpl::OnTitleChange(CefRefPtr<CefBrowser> browser,
    const CefString& title) {
    CEF_REQUIRE_UI_THREAD();
}