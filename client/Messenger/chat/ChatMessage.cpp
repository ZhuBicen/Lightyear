#include <fstream>
#include <Richedit.h>
#include <Ole2.h>
#include <RichOle.h>
#include <boost/foreach.hpp>
#include "ChatMessage.hpp"
#include "chat/SmileyWindow.hpp"
#include "ImageDataObject.hpp"
#include "DrawUtil.hpp"
#include "RosterMgmt.hpp"
#include "log.hpp"
// PlainText
std::wstring escapeHtml(const std::wstring& data) {
    std::wstring buffer;
    buffer.reserve(data.size());
    for (size_t pos = 0; pos != data.size(); ++pos) {
        switch (data[pos]) {
        case L'&':  buffer.append(L"&amp;");       break;
        case L'\"': buffer.append(L"&quot;");      break;
        case L'\'': buffer.append(L"&apos;");      break;
        case L'<':  buffer.append(L"&lt;");        break;
        case L'>':  buffer.append(L"&gt;");        break;
        default:   buffer.append(&data[pos], 1); break;
        }
    }
    return buffer;
}
std::wstring PlainText::AsHtml() const {
    return escapeHtml(m_text);
}
im::EnumMessageElement PlainText::Kind() const {
    return im::TEXT;
}
std::string PlainText::Data() const {
    return ConvertDuiToUtf8(m_text.c_str());
}

// Smiley
im::EnumMessageElement Smiley::Kind() const {
    return im::SMILEY;
}
std::string Smiley::Data() const {
    return ConvertDuiToUtf8(m_smileyFileName.c_str());
}
std::wstring Smiley::AsHtml() const {
    return L"<image src='lightyear://messagehistory/" +  m_smileyFileName + L"\'>";
}

// ImageElement
ImageElement::ImageElement(HBITMAP p_bitmap): m_bitmap(p_bitmap){
}
ImageElement::ImageElement(char* p_buffer, int p_bufferSize) {
    m_bitmap = CDrawUtil::Bytes2Bitmap(p_buffer, p_bufferSize);
}
ImageElement::ImageElement(const string& p_str){
    char* l_buffer = new char[p_str.length()];
    memcpy(l_buffer, p_str.c_str(), p_str.length());
    m_bitmap = CDrawUtil::Bytes2Bitmap(l_buffer, p_str.length());
    delete[] l_buffer;
};

std::wstring ImageElement::AsHtml() const {
    if (m_fileName.empty()) {
        m_fileName = GenerateScreenshotFileName();
        Save();
    }
    return L"<img src='" + m_fileName + L"' alt='Image Not Available!'>";
}
im::EnumMessageElement ImageElement::Kind() const {
    return im::BITMAP;
}
std::string ImageElement::Data() const {
    int l_length = 0;
    char* l_buffer = CDrawUtil::Bitmap2Bytes(m_bitmap, l_length);
    std::string l_retString;
    l_retString.append(l_buffer, l_length);
    delete[] l_buffer;
    return l_retString;
}
// save the bitmap to disk
void ImageElement::Save() const {
    std::ofstream ofs((GetBinaryPathW() + L"\\Resources\\" + m_fileName).c_str(), std::ofstream::binary);
    const std::string& d = Data();
    ofs.write(d.c_str(), d.length());
    ofs.close();
}

RichChatMessage::RichChatMessage(const std::wstring& p_messageText)
    :m_plainMessageText(p_messageText)
{
    parse();
}

RichChatMessage::RichChatMessage(const std::wstring& p_messageText, const std::vector< std::pair<int, HBITMAP> > & p_bitmaps)
{
    int l_currentPos = 0;
	typedef std::pair<int, HBITMAP> BitmapPair;
	BOOST_FOREACH(BitmapPair bitmap, p_bitmaps) {
        std::wstring l_section = p_messageText.substr(l_currentPos, bitmap.first - l_currentPos);
        l_currentPos = bitmap.first + 1; // skip the space
        this->push(new PlainText(l_section));
        auto smileyFileName = SmileyWindow::FindSmiley(bitmap.second);
        if (smileyFileName.empty()) {
            this->push(new ImageElement(bitmap.second));
        } else {
            this->push(new Smiley(smileyFileName));

        }
    }
    this->push(new PlainText(p_messageText.substr(l_currentPos, p_messageText.length() - l_currentPos)));
}


RichChatMessage::~RichChatMessage(void)
{
    for(MessageElement* l_messageElement: m_elements) {
        delete l_messageElement;
    }
}

std::wstring GetCurrentTimeString() {
    SYSTEMTIME time = { 0 };
    TCHAR szTime[MAX_PATH] = { 0 };
    ::GetLocalTime(&time);
    _stprintf_s(szTime, MAX_PATH, _T(" %04d-%02d-%02d %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
    return szTime;
}
std::wstring RichChatMessage::AsHtml() const {
    std::wstring htmlContent = L"<h2>";
    RosterItem ri = RosterMgmt::Instance()->GetRosterItem(m_from);
    htmlContent += ri.name ? (*ri.name + L"(" + ri.id + L")") : ri.id;
    htmlContent += L" " + GetCurrentTimeString();
    htmlContent += L"</h2><p>";
    for (const MessageElement* l_element: m_elements) {
        htmlContent += l_element->AsHtml();
    }
    htmlContent += L"</p>";
    return htmlContent;
}
void RichChatMessage::parse(void)
{
    size_t l_plainTextIndex = 0;
    for(size_t l_index = 0; l_index < m_plainMessageText.length(); /*blank*/) {
        bool l_smileyFound = false;
		typedef std::pair<std::wstring, boost::tuple<std::wstring, std::wstring, std::string> > SmileyPair;
        BOOST_FOREACH(SmileyPair l_emotion , SmileyWindow::s_smileyCollection) {
            if (l_index + l_emotion.first.length() <= m_plainMessageText.length() 
                && m_plainMessageText.substr(l_index, l_emotion.first.length()) == l_emotion.first) {

                    m_elements.push_back( new PlainText(
                        m_plainMessageText.substr(l_plainTextIndex, l_index - l_plainTextIndex)));

                    m_elements.push_back( new Smiley(l_emotion.first));

                    // advance the index
                    l_index += l_emotion.first.length();
                    l_plainTextIndex = l_index;

                    l_smileyFound = true;
                    break;
            }
        }
        if (!l_smileyFound) {
            // advance index
            l_index++;
        }
    }
    if (l_plainTextIndex < m_plainMessageText.length()) {
        m_elements.push_back( new PlainText(
            m_plainMessageText.substr(l_plainTextIndex, m_plainMessageText.length() - l_plainTextIndex)));
    }
}
