#undef NOMINMAX
#include "VideoWindow.hpp"

VideoWindow::VideoWindow(HWND hwnd)
{
    Create(hwnd, _T("VideoWindow"), UI_WNDSTYLE_FRAME, 0, 0, 0, 500, 500);
}


VideoWindow::~VideoWindow(void)
{
}

CControlUI* VideoWindow::CreateControl(LPCTSTR pstrClass){
    if (_tcsicmp(pstrClass, _T("LightyearVideoControl")) == 0)
    {
        return new VideoControl();
    }
    return NULL;
}
LPCTSTR VideoWindow::GetWindowClassName() const
{ 
    return _T("LIGHTYEAR_VIDEO_WINDOW");
}

void VideoWindow::OnFinalMessage(HWND hWnd)
{
    WindowImplBase::OnFinalMessage(hWnd);
    delete this;
}

void VideoWindow::Notify(TNotifyUI& msg) {
    if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
    {
        InitBackground();
        InitTitleBar();
    }
    if (msg.sType == _T("click"))
    {
        __super::OnClick(msg);
    }
}

CDuiString VideoWindow::GetSkinFile() const
{
    return _T("xml\\VideoWindow.xml");
}
void VideoWindow::InitBackground() {
    CHorizontalLayoutUI* bg = (CHorizontalLayoutUI*)m_PaintManager.FindControl(_T("LightyearBackground"));
    if (bg != NULL) {
        bg->SetBkImage(_T("file = 'bg2.png' corner = '600,200,1,1'"));
    }
    CVerticalLayoutUI* bd = (CVerticalLayoutUI*)m_PaintManager.FindControl(_T("LightyearBorder"));
    if (bd != NULL) {
        bd->SetBkImage(_T("file = 'border.png' corner = '5,5,5,5'"));
        bd->SetInset(RECT{ 1, 1, 1, 1 });
    }
}

void VideoWindow::InitTitleBar()
{
    CHorizontalLayoutUI* l_titleBar = (CHorizontalLayoutUI*)m_PaintManager.FindControl(_T("LightyearTitleBar"));
    if (l_titleBar == NULL) {
        return;
    }
    l_titleBar->SetFixedHeight(30);
    // l_titleBar->SetBorderColor(0xFFFFFFFF);
    // l_titleBar->SetBorderSize(3);

    // min button
    CButtonUI* l_minButton = new CButtonUI;
    l_minButton->SetFloat(true);
    l_minButton->SetName(_T("minbtn"));
    l_minButton->SetFixedXY(SIZE{ 0, 0 });
    l_minButton->SetFixedWidth(28);
    l_minButton->SetFixedHeight(19);
    l_minButton->SetMaxWidth(28);
    l_minButton->SetMaxHeight(19);
    l_minButton->SetNormalImage(_T("title\\btn_mini_normal.png"));
    l_minButton->SetHotImage(_T("title\\btn_mini_highlight.png"));
    l_minButton->SetPushedImage(_T("title\\btn_mini_down.png"));
    l_minButton->SetVisible(true);
    l_titleBar->Add(l_minButton);
}
// Video Control to render the Video stream

VideoControl::VideoControl() {
}

void VideoControl::DoPaint(HDC hDC, const RECT& rcPaint)
{
}


void VideoControl::DoEvent(TEventUI& event){
    __super::DoEvent(event);
}


