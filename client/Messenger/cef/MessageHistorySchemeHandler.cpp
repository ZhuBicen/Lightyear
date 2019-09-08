#include "MessageHistorySchemeHandler.hpp"
#include <regex>
#include <fstream>
#include "Utility.hpp"
#include "LocalProfile.hpp"
#include "log.hpp"

ClientSchemeHandler::ClientSchemeHandler() : offset_(0) {}

bool ClientSchemeHandler::ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) {
    CEF_REQUIRE_IO_THREAD();
    bool handled = false;
    std::string url = request->GetURL();
    std::smatch sm;
    MLOG(logDEBUG) << "Requested URL = " << url;
    if (std::regex_match(url, sm, std::regex("^lightyear://messagehistory/(.*bmp)$"))) {
        std::ifstream ifs;
        std::string emotionFileName = GetBinaryPathA() + "\\Resources\\" + sm.str(1);
        AppendFileContent(emotionFileName);
        mime_type_ = "image/bmp";
        handled = true;
    }
    else if (std::regex_match(url, sm, std::regex("^lightyear://messagehistory/css/message_styles.css$"))) {
        std::ifstream ifs;
        std::string css = GetBinaryPathA() + "\\Resources\\css\\message_styles.css";
        AppendFileContent(css);
        handled = true;
        mime_type_ = "text/css";
    }
    else if (std::regex_match(url, sm, std::regex("^lightyear://messagehistory/(.*)$"))) {
        // Build the response html
        data_ = "<meta charset=\"UTF-8\"><!DOCTYPE html> <html>";
        data_ += "<head><title>Message History</title>";
        // data_ += "<link rel=\"stylesheet\" href=\"file:///c:/message_styles.css\">";
        data_ += "<link rel=\"stylesheet\" href=\"";
        data_ += "css/message_styles.css\"";
        data_ += "</head>";
        data_.append("<body style = 'overflow:scroll;overflow-x:hidden;word-wrap : break-word;'><h1>");
        data_.append(ToString(L"与"));
        data_.append(sm[1]);
        data_.append(ToString(L"的聊天记录"));
        data_.append("</h1><div id=\"messages\">");
        for (auto msg : LocalProfile::Instance()->QueryChatMessagesById(ToWstring(sm[1]))) {
            data_.append(ToString(msg->AsHtml()));
        }
        data_.append("</div></body><script>window.scrollTo(0,document.body.scrollHeight); </script></html>");
        handled = true;
        // Set the resulting mime type
        mime_type_ = "text/html";
    }

    if (handled) {
        // Indicate the headers are available.
        callback->Continue();
        return true;
    }

    return false;
}

void ClientSchemeHandler::GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) {
    CEF_REQUIRE_IO_THREAD();

    DCHECK(!data_.empty());

    response->SetMimeType(mime_type_);
    response->SetStatus(200);

    // Set the resulting response length
    response_length = data_.length();
}

void ClientSchemeHandler::Cancel() {
    CEF_REQUIRE_IO_THREAD();
}

bool ClientSchemeHandler::ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) {
    CEF_REQUIRE_IO_THREAD();
    bool has_data = false;
    bytes_read = 0;

    if (offset_ < data_.length()) {
        // Copy the next block of data into the buffer.
        int transfer_size =
            min(bytes_to_read, static_cast<int>(data_.length() - offset_));
        memcpy(data_out, data_.c_str() + offset_, transfer_size);
        offset_ += transfer_size;

        bytes_read = transfer_size;
        has_data = true;
    }

    return has_data;
}
void ClientSchemeHandler::AppendFileContent(const std::string& file_name) {
    std::ifstream ifs;
    ifs.open(file_name.c_str(), ios::in | ios::binary);
    if (!ifs.is_open()) {
        MLOG(logERROR) << "Can't open file:" << file_name;
        return;
    }
    ifs.seekg(0, ifs.end);
    int length = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char* buffer = new char[length];
    ifs.read(buffer, length);
    ifs.close();
    data_.append((char*)buffer, length);
    delete[] buffer;
}