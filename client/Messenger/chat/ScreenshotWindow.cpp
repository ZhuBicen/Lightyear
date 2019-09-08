#include "chat/ScreenshotWindow.hpp"
#include <iostream>
#include "UiMessage.hpp"
#include "Log.hpp"
#include "DrawUtil.hpp"

ScreenshotWindow::ScreenshotWindow(HWND p_chatDialog):m_chatDialog(p_chatDialog)
{
    Create(NULL, _T("SCREENSHOT"), WS_POPUP, WS_EX_TOOLWINDOW, 0, 0);
    ShowWindow(true);
}


ScreenshotWindow::~ScreenshotWindow(void)
{
}

CControlUI* ScreenshotWindow::CreateControl(LPCTSTR pstrClass){
    if (_tcsicmp(pstrClass, _T("ClientPainter")) == 0)
    {
        return new ClientPainter(this, m_chatDialog);
    }

    return NULL;
}
LPCTSTR ScreenshotWindow::GetWindowClassName() const 
{ 
    return _T("SCREENSHOT_WINDOW");
}

void ScreenshotWindow::OnFinalMessage(HWND hWnd)
{
    WindowImplBase::OnFinalMessage(hWnd);
    delete this;
}

void ScreenshotWindow::Notify(TNotifyUI& msg) {

}
void ScreenshotWindow::InitWindow()
{
    HDC hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL); 
    int width  = GetDeviceCaps(hScrDC, HORZRES); 
    int height = GetDeviceCaps(hScrDC, VERTRES); 
    MoveWindow(m_hWnd, 0, 0, width, height, FALSE);

    if (0 == SetForegroundWindow(m_hWnd)) {
        MLOG(logDEBUG) << "SetForgroundWindow failure.";
    }
    if (0 == SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE)) {
        MLOG(logDEBUG) << "SetWindowPos failure.";
    }
    DeleteDC(hScrDC); 
}

CDuiString ScreenshotWindow::GetSkinFile() const
{
    return _T("xml\\ScreenshotWindow.xml");
}

HBITMAP ScreenshotWindow::CopyScreen(void)
{
    HDC hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL); 
    HDC hMemDC = CreateCompatibleDC(hScrDC); 
    int width  = GetDeviceCaps(hScrDC, HORZRES); 
    int height = GetDeviceCaps(hScrDC, VERTRES); 
    HBITMAP hBitmap = CreateCompatibleBitmap(hScrDC, width, height); 
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 
    BitBlt(hMemDC, 0, 0, width, height,hScrDC, 0, 0, SRCCOPY); 
    hBitmap =(HBITMAP)SelectObject(hMemDC, hOldBitmap); 
    DeleteDC(hScrDC); 
    DeleteDC(hMemDC); 
    return hBitmap; 
}

ClientPainter::ClientPainter(ScreenshotWindow* p_parentWindow, HWND p_chatDialog):
    m_parentWindow(p_parentWindow), m_chatDialog(p_chatDialog) {
    m_screenRect.left = m_screenRect.top = 0;
    HDC hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL); 
    m_screenRect.right  = GetDeviceCaps(hScrDC, HORZRES); 
    m_screenRect.bottom = GetDeviceCaps(hScrDC, VERTRES); 
    DeleteDC(hScrDC); 
}

void ClientPainter::DoPaint(HDC hDC, const RECT& rcPaint)
{

    MLOG(logDEBUG) << __FUNCTION__;
    RECT corner = {0, 0, 0, 0};
    m_bitmap = ScreenshotWindow::CopyScreen();
    CRenderEngine::DrawColor(hDC, m_screenRect, 0xFF000000);
    CRenderEngine::DrawImage(hDC, m_bitmap, m_screenRect, m_screenRect, m_screenRect,  corner, false, 80);

    // CControlUI::DoPaint(hDC, rcPaint);
    // CRenderEngine::DrawImageString(m_PaintManager.GetPaintDC(), &m_PaintManager, rc, rc, _T("bg.bmp"), _T("corner=4,4,2,2"));
}
std::ostream& operator<< (std::ostream& out, RECT rc) {
    return out << "(" << rc.left << "," << rc.top << "," << rc.right << "," << rc.bottom << ")";
}
std::ostream& operator<< (std::ostream& out, POINT pt) {
    return out << "(" << pt.x << "," << pt.y << ")";
}

RECT caculateRect(POINT p_startPoint, POINT p_endPoint) {
    // MLOG(logDEBUG) << __FUNCTION__ << "StartPoint:" << p_startPoint;
    // MLOG(logDEBUG) << __FUNCTION__ << "EndPoint:  " << p_endPoint;
    RECT l_result = {0, 0, 0, 0};

    if (p_endPoint.x >= p_startPoint.x && p_endPoint.y >= p_startPoint.y) {
        l_result.left = p_startPoint.x;
        l_result.top = p_startPoint.y;
        l_result.right = p_endPoint.x;
        l_result.bottom = p_endPoint.y;
    } else if (p_endPoint.x >= p_startPoint.x && p_endPoint.y <= p_startPoint.y) {
        l_result.left = p_startPoint.x;
        l_result.top = p_endPoint.y;
        l_result.right = p_endPoint.x;
        l_result.bottom = p_startPoint.y;
    } else if (p_endPoint.x <= p_startPoint.x && p_endPoint.y <= p_startPoint.y) {
        l_result.left = p_endPoint.x;
        l_result.top = p_endPoint.y;
        l_result.right = p_startPoint.x;
        l_result.bottom = p_startPoint.y;
    } else if (p_endPoint.x <= p_startPoint.x && p_endPoint.y >= p_startPoint.y) {
        l_result.left = p_endPoint.x;
        l_result.top = p_startPoint.y;
        l_result.right = p_startPoint.x;
        l_result.bottom = p_endPoint.y;
    } else {
        ASSERT(false);
        MLOG(logERROR) << __FUNCTION__ << "ERROR, fail to calculate RECT.";
    }

    if (l_result.right < l_result.left || l_result.bottom < l_result.top) {
        MLOG(logERROR) << __FUNCTION__ << " ERROR, invalid rect.";
    }
    return l_result;
}

void ClientPainter::DoEvent(TEventUI& event){
    if (event.Type == UIEVENT_BUTTONDOWN) {
        MLOG(logDEBUG) << __FUNCTION__ << " left button down";
        m_startPoint = event.ptMouse;
        return;
    }
    if (event.Type == UIEVENT_BUTTONUP) {
        m_parentWindow->Close();
        PostMessage(m_chatDialog, WM_SCREENSHOT, (WPARAM)GetSelectedBitmap(), 0);
        m_startPoint.reset();
        m_endPoint.reset(); 
        return;
    }
    if (event.Type == UIEVENT_MOUSEMOVE && (event.wParam & MK_LBUTTON)) {
        if (!m_startPoint) {
            return;
        }

        if (!m_endPoint) { // this is the first mouse move.
            m_endPoint = event.ptMouse;
            DrawSelected(caculateRect(*m_startPoint, *m_endPoint));
            return;
        }
        if (m_endPoint) {
            RECT l_previousSelected = caculateRect(*m_startPoint, *m_endPoint);
            m_endPoint = event.ptMouse;
            RECT l_newSelected = caculateRect(*m_startPoint, *m_endPoint);

            // 对从左上角到右下角的情况优化，避免屏幕闪烁
            if (l_newSelected.left == l_previousSelected.left && l_newSelected.top == l_previousSelected.top) {
                if (l_newSelected.bottom >= l_previousSelected.bottom && l_newSelected.right >= l_previousSelected.right){
                    {
                        RECT rc;
                        rc.left = l_newSelected.left;
                        rc.top = l_newSelected.top;
                        rc.bottom = l_newSelected.bottom;
                        rc.right = l_newSelected.right;
                        DrawSelected(rc);
                    }
                    {
                        RECT rc;
                        rc.left = l_previousSelected.right;
                        rc.top = l_previousSelected.top;
                        rc.right = l_newSelected.right;
                        rc.bottom = l_previousSelected.bottom;
                        DrawSelected(rc);
                    }
                    return;
                }
                if (l_newSelected.bottom < l_previousSelected.bottom && l_newSelected.right < l_previousSelected.right){
                    {
                        RECT rc;
                        rc.left = l_newSelected.left;
                        rc.top = l_newSelected.bottom;
                        rc.bottom = l_previousSelected.bottom;
                        rc.right = l_previousSelected.right;
                        DrawBackground(rc);
                    }
                    {
                        RECT rc;
                        rc.left = l_newSelected.right;
                        rc.top = l_previousSelected.top;
                        rc.right = l_previousSelected.right;
                        rc.bottom = l_newSelected.bottom;
                        DrawBackground(rc);
                    }
                    return;
                }

            }

            DrawBackground(l_previousSelected);

            DrawSelected(l_newSelected);
            return;
        }
    }
}

void ClientPainter::DrawBackground( RECT rc )
{
    RECT corner = {0, 0, 0, 0};
    CRenderEngine::DrawColor(m_pManager->GetPaintDC(), rc, 0xFF000000);
    CRenderEngine::DrawImage(m_pManager->GetPaintDC(), m_bitmap, rc, rc, rc,  corner, false, 80);
}

void ClientPainter::DrawSelected( RECT rect )
{
    RECT corner = {0, 0, 0, 0};
    CRenderEngine::DrawImage(m_pManager->GetPaintDC(), m_bitmap, rect, rect, rect,  corner, false);
}

// According to the startPoint and endPoint, and m_bitmap
// calculate the corresponding rect
HBITMAP ClientPainter::GetSelectedBitmap(void)
{
    HDC l_srcDc = ::CreateCompatibleDC(NULL);
    ::SelectObject(l_srcDc, m_bitmap);

    HDC l_dstDc = ::CreateCompatibleDC(l_srcDc);
    RECT l_rc = caculateRect(*m_startPoint, *m_endPoint);
    int width = l_rc.right - l_rc.left;
    int height = l_rc.bottom - l_rc.top;
    HBITMAP l_newBitmap = CreateCompatibleBitmap(l_srcDc, width, height);
    HBITMAP l_oldBitmap = (HBITMAP)::SelectObject(l_dstDc, l_newBitmap);
    if (0 == ::BitBlt(l_dstDc, 0, 0, width, height, l_srcDc, m_startPoint->x, m_startPoint->y, SRCCOPY))
    {
        MLOG(logERROR) << "BitBlt failed.";
        return m_bitmap;
    }
    
    HBITMAP l_clippedBitmap = (HBITMAP)::SelectObject(l_dstDc, l_oldBitmap);
    ::DeleteDC(l_srcDc);
    ::DeleteDC(l_dstDc);
    return l_clippedBitmap;

}
