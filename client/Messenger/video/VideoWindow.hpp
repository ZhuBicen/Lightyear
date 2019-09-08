#ifndef __LIGHTYEAR_MESSENGER_VIDEO_WINDOW__
#define __LIGHTYEAR_MESSENGER_VIDEO_WINDOW__
#include <functional>
#include "window/LightyearWindowBase.hpp"
#include "video/VideoWndInterface.hpp"
#include "proto/src/core.pb.h"

namespace p2p {
    class JingleMessage;
}
class MessageQueue;
class Conductor;

class VideoWindow : public LightyearWinodwBase, public VideoWndInterface
{
    friend class VideoControl;
public:
    VideoWindow(bool initialSide, HWND hwnd, const core::Jid& rosterNumber, MessageQueue& mq, std::function<void(VideoWindow*)> callback = nullptr);
    ~VideoWindow(void);

    LPCTSTR GetWindowClassName() const;
    virtual CControlUI* CreateControl(LPCTSTR pstrClass);
    virtual void OnFinalMessage(HWND hWnd);
    virtual void InitWindow();

    void Notify(TNotifyUI& msg);

    // implement LightyearWindowBase
    virtual CDuiString GetWindowText() const;
    virtual CDuiString GetSkinFile() const;
    virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


    // implement VideoWndInterface
    virtual void StartLocalRenderer(webrtc::VideoTrackInterface* local_video);
    virtual void StopLocalRenderer();
    virtual void StartRemoteRenderer(webrtc::VideoTrackInterface* remote_video);
    virtual void StopRemoteRenderer();
    void QueueUIThreadCallback(int msg_id, void* data);
    virtual void Exit(){};

    // TODO: Paint the image get from render
    void OnPaint(){}
    core::Jid RosterNumber();

    void ProcessJingleMessage(p2p::JingleMessage* p_message);
private:
    core::Jid roster_number_;
    VideoControl* video_control_;
    MessageQueue& message_queue_;
    bool initial_side_;
    std::function<void(VideoWindow*)> close_callback_;
};


#endif
