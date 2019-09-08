#ifndef GALLERY_08_MESSAGE_HISTORY_SCHEME_HANDLER_HPP
#define GALLERY_08_MESSAGE_HISTORY_SCHEME_HANDLER_HPP

#include <algorithm>
#include <string>

#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_frame.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_request.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"


// Implementation of the schema handler for client:// requests.
class ClientSchemeHandler : public CefResourceHandler {
public:
    ClientSchemeHandler();

    virtual bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) OVERRIDE;
    virtual void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) OVERRIDE;
    virtual void Cancel() OVERRIDE;
    virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) OVERRIDE;

private:
    // read the file content, and append the content to data_
    void AppendFileContent(const std::string& file_name);

    std::string data_;
    std::string mime_type_;
    size_t offset_;

    IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
};

// Implementation of the factory for for creating schema handlers.
class MessageHistorySchemeHandlerFactory : public CefSchemeHandlerFactory {
public:
    // Return a new scheme handler instance to handle the request.
    virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& scheme_name,
        CefRefPtr<CefRequest> request)
        OVERRIDE{
        CEF_REQUIRE_IO_THREAD();
        return new ClientSchemeHandler();
    }

    IMPLEMENT_REFCOUNTING(MessageHistorySchemeHandlerFactory);
};
#endif