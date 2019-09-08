#include "video/VideoWindow.hpp"
#include "Video/Conductor.hpp"
#include "UiMessage.hpp"
#include "Log.hpp"
#include "proto/src/p2p.pb.h"
#include "MessageQueue.hpp"

#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/base/scoped_ref_ptr.h"
#include "talk/base/scoped_ptr.h"
#include "talk/media/base/mediachannel.h"
#include "talk/media/base/videocommon.h"
#include "talk/media/base/videoframe.h"
#include "talk/media/base/videorenderer.h"
//
// VideoControl to render the Video Stream
// 
class VideoControl : public DuiLib::CControlUI {
public:
    VideoControl(VideoWindow*);
    virtual void DoPaint(HDC hDC, const RECT& rcPaint);
    virtual void SetFocus() {}
    virtual void DoEvent(TEventUI& event);

    void StartLocalRenderer(webrtc::VideoTrackInterface* local_video) {
        local_renderer_.reset(new VideoRenderer(this, 1, 1, local_video));
    }
    void StopLocalRenderer() {
        local_renderer_.reset();
    }
    void StartRemoteRenderer(webrtc::VideoTrackInterface* local_video) {
        remote_renderer_.reset(new VideoRenderer(this, 1, 1, local_video));
    }
    void StopRemoteRenderer() {
        remote_renderer_.reset();
    }
    void ProcessJingleMessage(p2p::JingleMessage* p_message);

    class VideoRenderer : public webrtc::VideoRendererInterface {
    public:
        VideoRenderer(VideoControl* vc, int width, int height,
            webrtc::VideoTrackInterface* track_to_render);
        virtual ~VideoRenderer();

        void Lock() {
            ::EnterCriticalSection(&buffer_lock_);
        }

        void Unlock() {
            ::LeaveCriticalSection(&buffer_lock_);
        }

        // VideoRendererInterface implementation
        virtual void SetSize(int width, int height);
        virtual void RenderFrame(const cricket::VideoFrame* frame);

        const BITMAPINFO& bmi() const { return bmi_; }
        const uint8* image() const { return image_.get(); }

    protected:
        enum {
            SET_SIZE,
            RENDER_FRAME,
        };

        VideoControl* vc_;
        BITMAPINFO bmi_;
        talk_base::scoped_ptr<uint8[]> image_;
        CRITICAL_SECTION buffer_lock_;
        talk_base::scoped_refptr<webrtc::VideoTrackInterface> rendered_track_;
    };

    // A little helper class to make sure we always to proper locking and
    // unlocking when working with VideoRenderer buffers.
    template <typename T>
    class AutoLock {
    public:
        explicit AutoLock(T* obj) : obj_(obj) { obj_->Lock(); }
        ~AutoLock() { obj_->Unlock(); }
    protected:
        T* obj_;
    };
private:
    talk_base::scoped_ptr<VideoRenderer> local_renderer_;
    talk_base::scoped_ptr<VideoRenderer> remote_renderer_;

    VideoWindow* video_window_;
    talk_base::scoped_refptr<Conductor> conductor_;
};

VideoWindow::VideoWindow(bool initialSide, HWND hwnd, const core::Jid& rosterNumber, MessageQueue& mq, std::function<void(VideoWindow*)> callback):
roster_number_(rosterNumber),
close_callback_(callback),
message_queue_(mq),
initial_side_(initialSide)
{
    Create(NULL, _T("VideoWindow"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE, 0, 0, 500, 500);
}


VideoWindow::~VideoWindow(void)
{
}
core::Jid VideoWindow::RosterNumber()
{
    return roster_number_;
}
void VideoWindow::InitWindow() {
    video_control_ = (VideoControl*)m_PaintManager.FindControl(L"videoControl");
    assert(video_control_ != NULL);

}
CControlUI* VideoWindow::CreateControl(LPCTSTR pstrClass){
    if (_tcsicmp(pstrClass, _T("LightyearVideoControl")) == 0)
    {
        return new VideoControl(this);
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
    if (msg.sType == _T("click"))
    {
        if (msg.pSender->GetName() == _T("closebtn") && close_callback_ != nullptr) {
            close_callback_(this);
        }
        __super::OnClick(msg);
    }
}
CDuiString VideoWindow::GetWindowText() const {
    return _T("视频 ...");
}
CDuiString VideoWindow::GetSkinFile() const
{
    return _T("xml\\VideoWindow.xml");
}

void VideoWindow::StartLocalRenderer(webrtc::VideoTrackInterface* local_video) {
    video_control_->StartLocalRenderer(local_video);
}

void VideoWindow::StopLocalRenderer() {
    video_control_->StopLocalRenderer();
}

void VideoWindow::StartRemoteRenderer(webrtc::VideoTrackInterface* remote_video) {
    video_control_->StartRemoteRenderer(remote_video);
}

void VideoWindow::StopRemoteRenderer() {
    video_control_->StopRemoteRenderer();
}

void VideoWindow::ProcessJingleMessage(p2p::JingleMessage* p_message) {
    video_control_->ProcessJingleMessage(p_message);

}

void VideoWindow::QueueUIThreadCallback(int msg_id, void* data){
    if (msg_id == Conductor::NEW_STREAM_ADDED) {
        ::PostMessage(m_hWnd, WM_INTERNAL_MESSAGE, (WPARAM)MessageType::StreamAdded, (LPARAM)data);
    }
    if (msg_id == Conductor::STREAM_REMOVED) {
        ::PostMessage(m_hWnd, WM_INTERNAL_MESSAGE, (WPARAM)MessageType::StreamRemoved, (LPARAM)data);
    }
};

LRESULT VideoWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
    if (uMsg == WM_INTERNAL_MESSAGE && (MessageType)wParam == MessageType::StreamAdded) {
        webrtc::MediaStreamInterface* stream =
            reinterpret_cast<webrtc::MediaStreamInterface*>(lParam);
        webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
        // Only render the first track.
        if (!tracks.empty()) {
            webrtc::VideoTrackInterface* track = tracks[0];
            video_control_->StartRemoteRenderer(track);
        }
        stream->Release();
        bHandled = TRUE;
        return 0;
    }
    if (uMsg == WM_INTERNAL_MESSAGE && (MessageType)wParam == MessageType::StreamRemoved) {
        webrtc::MediaStreamInterface* stream =
            reinterpret_cast<webrtc::MediaStreamInterface*>(lParam);
        stream->Release();
        bHandled = TRUE;
        return 0;
    }
    if (uMsg == WM_INTERNAL_MESSAGE && (MessageType)wParam == MessageType::RenderVideo) {
        video_control_->Invalidate();
    }
    bHandled = FALSE;
    return 0;
}
//
// VideoWindow::VideoRenderer
//

VideoControl::VideoRenderer::VideoRenderer(
    VideoControl* vc, int width, int height,
    webrtc::VideoTrackInterface* track_to_render)
    :vc_(vc), rendered_track_(track_to_render) {
    ::InitializeCriticalSection(&buffer_lock_);
    ZeroMemory(&bmi_, sizeof(bmi_));
    bmi_.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi_.bmiHeader.biPlanes = 1;
    bmi_.bmiHeader.biBitCount = 32;
    bmi_.bmiHeader.biCompression = BI_RGB;
    bmi_.bmiHeader.biWidth = width;
    bmi_.bmiHeader.biHeight = -height;
    bmi_.bmiHeader.biSizeImage = width * height *
        (bmi_.bmiHeader.biBitCount >> 3);
    rendered_track_->AddRenderer(this);
}

VideoControl::VideoRenderer::~VideoRenderer() {
    rendered_track_->RemoveRenderer(this);
    ::DeleteCriticalSection(&buffer_lock_);
}

void VideoControl::VideoRenderer::SetSize(int width, int height) {
    AutoLock<VideoRenderer> lock(this);

    bmi_.bmiHeader.biWidth = width;
    bmi_.bmiHeader.biHeight = -height;
    bmi_.bmiHeader.biSizeImage = width * height *
        (bmi_.bmiHeader.biBitCount >> 3);
    image_.reset(new uint8[bmi_.bmiHeader.biSizeImage]);
}

void VideoControl::VideoRenderer::RenderFrame(const cricket::VideoFrame* frame) {
    if (!frame)
        return;

    {
        AutoLock<VideoRenderer> lock(this);

        ASSERT(image_.get() != NULL);
        frame->ConvertToRgbBuffer(cricket::FOURCC_ARGB,
            image_.get(),
            bmi_.bmiHeader.biSizeImage,
            bmi_.bmiHeader.biWidth *
            bmi_.bmiHeader.biBitCount / 8);
    }
    // Post message to GUI thread to invalidate the control
    ::PostMessage(*vc_->video_window_, WM_INTERNAL_MESSAGE, (LPARAM)MessageType::RenderVideo, 0);
}


// Video Control to render the Video stream

VideoControl::VideoControl(VideoWindow* vw) :
video_window_(vw),
conductor_(
    new talk_base::RefCountedObject<Conductor>(video_window_->RosterNumber(),
    std::ref(video_window_->message_queue_), video_window_)) {
    
}

void VideoControl::DoPaint(HDC hDC, const RECT& rc)
{

    static bool intial = false;
    if (video_window_->initial_side_ && !intial) {
        conductor_->Connect();
        intial = true;
    }
    __super::DoPaint(hDC, rc);

    VideoRenderer* local_renderer = local_renderer_.get();
    VideoRenderer* remote_renderer = remote_renderer_.get();

    if (!local_renderer_ || !remote_renderer_) {
        return;
    }
    const BITMAPINFO& bmi = remote_renderer->bmi();
    int height = abs(bmi.bmiHeader.biHeight);
    int width = bmi.bmiHeader.biWidth;

    const uint8* image = remote_renderer->image();
    if (image == NULL) {
        return;
    }
    HDC dc_mem = ::CreateCompatibleDC(hDC);
    ::SetStretchBltMode(dc_mem, HALFTONE);

    // Set the map mode so that the ratio will be maintained for us.
    HDC all_dc[] = { hDC, dc_mem };
    for (int i = 0; i < ARRAY_SIZE(all_dc); ++i) {
        SetMapMode(all_dc[i], MM_ISOTROPIC);
        SetWindowExtEx(all_dc[i], width, height, NULL);
        SetViewportExtEx(all_dc[i], rc.right, rc.bottom, NULL);
    }

    HBITMAP bmp_mem = ::CreateCompatibleBitmap(hDC, rc.right, rc.bottom);
    HGDIOBJ bmp_old = ::SelectObject(dc_mem, bmp_mem);

    POINT logical_area = { rc.right, rc.bottom };
    DPtoLP(hDC, &logical_area, 1);

    HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0));
    RECT logical_rect = { 0, 0, logical_area.x, logical_area.y };
    ::FillRect(dc_mem, &logical_rect, brush);
    ::DeleteObject(brush);

    int x = (logical_area.x / 2) - (width / 2);
    int y = (logical_area.y / 2) - (height / 2);

    StretchDIBits(dc_mem, x, y, width, height,
        0, 0, width, height, image, &bmi, DIB_RGB_COLORS, SRCCOPY);

    if ((rc.right - rc.left) > 200 && (rc.bottom - rc.top) > 200) {
        const BITMAPINFO& bmi = local_renderer->bmi();
        image = local_renderer->image();
        int thumb_width = bmi.bmiHeader.biWidth / 4;
        int thumb_height = abs(bmi.bmiHeader.biHeight) / 4;
        StretchDIBits(dc_mem,
            logical_area.x - thumb_width - 10,
            logical_area.y - thumb_height - 10,
            thumb_width, thumb_height,
            0, 0, bmi.bmiHeader.biWidth, -bmi.bmiHeader.biHeight,
            image, &bmi, DIB_RGB_COLORS, SRCCOPY);
    }

    BitBlt(hDC, 0, 0, logical_area.x, logical_area.y,
        dc_mem, 0, 0, SRCCOPY);

    // Cleanup.
    ::SelectObject(dc_mem, bmp_old);
    ::DeleteObject(bmp_mem);
    ::DeleteDC(dc_mem);
    MLOG(logDEBUG) << __FUNCTION__ << "(" << rc.left << ", " << rc.top
        << ", " << rc.right << ", " << rc.bottom << ")";
}


void VideoControl::DoEvent(TEventUI& event){
    __super::DoEvent(event);
}

void VideoControl::ProcessJingleMessage(p2p::JingleMessage* p_message){
    conductor_->OnMessageFromPeer(p_message->content());
}

