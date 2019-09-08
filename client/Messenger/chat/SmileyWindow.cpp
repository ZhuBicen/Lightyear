#include <windows.h>
#include <shellapi.h>
#include "Log.hpp"
#include "chat/SmileyWindow.hpp"
#include "chat/ChatWindow.hpp"
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>





using namespace boost::assign; // bring 'map_list_of()' into scope

SmileyWindow::SmileyWindow(ChatWindow* chat_dialog, POINT ptMouse)
: based_point_(ptMouse)
, chat_dialog_(chat_dialog)
{
	Create(NULL, _T("color"), WS_POPUP, WS_EX_TOOLWINDOW, 0, 0);
	ShowWindow(true);
}

LPCTSTR SmileyWindow::GetWindowClassName() const 
{ 
	return _T("EmotionSelectorWindow");
}

void SmileyWindow::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void SmileyWindow::Notify(TNotifyUI& msg)
{
    
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
        std::wstring l_emotionImage = static_cast<CButtonUI*>(msg.pSender)->GetNormalImage();
        chat_dialog_->InsertEmotion(l_emotionImage);

        MLOG(logINFO) << " You selected emotion:" << ToString(l_emotionImage);
        Close(0);

	}
    else if ( _tcsicmp(msg.sType, _T("itemactivate")) == 0 || _tcsicmp(msg.sType, _T("itemclick")) == 0)
    {
        auto l_listItem = static_cast<CListContainerElementUI*>(msg.pSender);
        auto l_imageButton = static_cast<CButtonUI*> (m_PaintManager.FindSubControlByName(l_listItem, _T("emotionImageButton")));
        chat_dialog_->InsertEmotion(l_imageButton->GetNormalImage());

        MLOG(logINFO) << "You selected emotion:" << ConvertDuiToUtf8(l_imageButton->GetNormalImage());
        Close(0);
    }
}



void SmileyWindow::InitWindow()
{
	CListUI* l_emotionContainer = static_cast<CListUI*>(m_PaintManager.FindControl(_T("emotionCollection")));
	for (SmileyMap::const_iterator l_emotion = s_smileyCollection.begin();
		l_emotion != s_smileyCollection.end(); l_emotion++) {
		CListContainerElementUI* l_listItem = new CListContainerElementUI();
        CHorizontalLayoutUI* l_line = new CHorizontalLayoutUI;
		l_line->SetFixedHeight(24);
        // image
        CButtonUI* l_emotionImage = new CButtonUI();
        l_emotionImage->SetName(_T("emotionImageButton"));
        l_emotionImage->SetNormalImage(l_emotion->second.get<1>().c_str());
        l_emotionImage->SetFixedHeight(24);
        l_emotionImage->SetFixedWidth(24);
        l_line->Add(l_emotionImage);
        // text
        CLabelUI* l_emotionText = new CLabelUI();
        l_emotionText->SetText(l_emotion->first.c_str());
        l_line->Add(l_emotionText);
        // meaning
        CLabelUI* l_emotionMeaning = new CLabelUI;
        l_emotionMeaning->SetText(l_emotion->second.get<0>().c_str());
        l_line->Add(l_emotionMeaning);

        l_listItem->Add(l_line);
        l_emotionContainer->Add(l_listItem);
    }
	SIZE size = m_PaintManager.GetInitSize();
	MoveWindow(GetHWND(), based_point_.x - static_cast<LONG>(size.cx), based_point_.y - size.cy, size.cx, size.cy, FALSE);
}

CDuiString SmileyWindow::GetSkinFile() const
{
	return _T("xml\\SmileyWindow.xml");
}

LRESULT SmileyWindow::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}

std::string TextPresentation(const std::wstring& p_emotionPath) {
    return SmileyWindow::EncodeHbitmap(CRenderEngine::LoadImage(p_emotionPath.c_str())->hBitmap);
}

SmileyWindow::SmileyMap SmileyWindow::s_smileyCollection = {
        // TODO: 惊讶 吐舌头 说错话 尴尬
        { _T(":-)"),  boost::make_tuple<std::wstring, std::wstring, std::string>(_T("微笑"), _T("emotion/face-smile-24.bmp"), "") },
        { _T(";-)"),  boost::make_tuple<std::wstring, std::wstring, std::string>(_T("眨眼"), _T("emotion/face-wink-24.bmp"), "") },
        { _T(":-("),  boost::make_tuple<std::wstring, std::wstring, std::string>(_T("委屈"), _T("emotion/face-sad-24.bmp"), "") },
        { _T(">:o"),  boost::make_tuple<std::wstring, std::wstring, std::string>(_T("生气"), _T("emotion/face-angel-24.bmp"), "") },
        { _T(":-D"),  boost::make_tuple<std::wstring, std::wstring, std::string>(_T("大笑"), _T("emotion/face-grin-24.bmp"), "") },
        { _T("o;-)"), boost::make_tuple<std::wstring, std::wstring, std::string>(_T("无辜"), _T("emotion/face-plain-24.bmp"), "") }
    };
void SmileyWindow::Init() {
    // s_smileyCollection = 
};
std::wstring SmileyWindow::FindSmiley(HBITMAP p_hbitmap) {
    auto base64String = EncodeHbitmap(p_hbitmap);
    // loop emotion to get Smiley
	// Why is this not working?
	// BOOST_FOREACH (SmileyPair& e, s_smileyCollection) {
    BOOST_FOREACH (SmileyPair& e, s_smileyCollection) {
        if (e.second.get<2>() == "") {
            e.second.get<2>() = TextPresentation(e.second.get<1>());
        }
        if (e.second.get<2>() == base64String) {
            return e.second.get<1>();
        }
    }

    MLOG(logERROR) << "can't get the smiley.";
    return _T("");
}

std::string SmileyWindow::EncodeHbitmap(HBITMAP p_hbitmap) {
    HDC dcBitmap = CreateCompatibleDC ( NULL );
    // SelectObject( dcBitmap, p_hbitmap );

    BITMAPINFO bmpInfo = {0};
    ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
    int nRet = ::GetDIBits(dcBitmap, p_hbitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS); 
    if (nRet == 0) {
        nRet = GetLastError();
        MLOG(logDEBUG) << __FUNCTION__ << "GetDIBits for bmpInfo failed" << nRet;
        return "";
    }
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    if (bmpInfo.bmiHeader.biHeight > 0 ) {
        bmpInfo.bmiHeader.biHeight = -bmpInfo.bmiHeader.biHeight;
    }
    // COLORREF* pixel = new COLORREF [ bmpInfo2.bmiHeader.biSizeImage / sizeof(COLORREF) ];
    BYTE* lpvBits = new BYTE[bmpInfo.bmiHeader.biSizeImage];
    ::GetDIBits(dcBitmap, p_hbitmap, 0, bmpInfo.bmiHeader.biHeight > 0 ? bmpInfo.bmiHeader.biHeight: -bmpInfo.bmiHeader.biHeight,
        lpvBits, &bmpInfo, DIB_RGB_COLORS);
    return Base64Enocde(lpvBits, bmpInfo.bmiHeader.biSizeImage);
}
