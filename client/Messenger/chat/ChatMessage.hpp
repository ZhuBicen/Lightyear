#ifndef LIGHTYEAR_MESSENGER_CHAT_MESSAGE_HPP
#define LIGHTYEAR_MESSENGER_CHAT_MESSAGE_HPP

#include <boost/shared_ptr.hpp>
#include "proto/src/im.pb.h"
#include "Utility.hpp"

class MessageElement {
public:
    virtual std::wstring AsHtml() const = 0;
    virtual im::EnumMessageElement  Kind() const = 0;
    virtual std::string Data() const = 0;
};

class PlainText: public MessageElement {
public:
    explicit PlainText(const std::wstring p_text):m_text(p_text) {}
    std::wstring AsHtml() const;
    im::EnumMessageElement Kind() const ;
    std::string Data() const;

private:
    std::wstring m_text;
};

class Smiley : public MessageElement{
public:
    explicit Smiley(std::wstring p_smileyFileName): m_smileyFileName(p_smileyFileName) {}
    std::wstring AsHtml() const;
    im::EnumMessageElement Kind() const;
    std::string Data() const;

private:
    std::wstring m_smileyFileName;
};

class ImageElement: public MessageElement {

public:
    explicit ImageElement(HBITMAP p_bitmap);
    explicit ImageElement(char* p_buffer, int p_bufferSize);
    explicit ImageElement(const string& p_str);
    std::wstring AsHtml() const;
    im::EnumMessageElement Kind() const;
    std::string Data() const;
    void Save() const;

private:
    HBITMAP m_bitmap;
    mutable std::wstring m_fileName;
};

class RichChatMessage
{
public:
    RichChatMessage(const std::wstring& p_messageText);
    RichChatMessage(const std::wstring& p_messageText, const std::vector< std::pair<int, HBITMAP> > & p_bitmaps);
    RichChatMessage(){};
    ~RichChatMessage() ;
    void push(MessageElement* p_messageElement) {
        m_elements.push_back((p_messageElement));
    }
    const std::vector< MessageElement* >& Elements() const {
        return m_elements;
    }

    void SetFrom(std::wstring p_from) {
        m_from = p_from;
    }
    std::wstring From() {
        return m_from;
    }
    std::wstring To() {
        return m_to;
    }
    void SetTo(std::wstring p_to) {
        m_to = p_to;
    }
private:
    std::vector< MessageElement* > m_elements;
    std::wstring m_plainMessageText;
    void parse(void);  

    std::wstring m_from;
    std::wstring m_to;

public:
    std::wstring AsHtml() const;
};

typedef boost::shared_ptr<RichChatMessage> CChatMessagePtr;

#endif
