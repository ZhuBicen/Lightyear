#ifndef LIGHTYEAR_VIDEO_WND_CALLBACK_HPP
#define LIGHTYEAR_VIDEO_WND_CALLBACK_HPP
namespace webrtc{
	class VideoTrackInterface;
}

class VideoWndInterface {
public:
	virtual void StartLocalRenderer(webrtc::VideoTrackInterface*) = 0;
	virtual void StartRemoteRenderer(webrtc::VideoTrackInterface*) = 0;
	virtual void StopLocalRenderer() = 0;
	virtual void StopRemoteRenderer() = 0;
	virtual void QueueUIThreadCallback(int msg_id, void* data) = 0;

	// the p2p disconnected.
	virtual void Exit() = 0;
};
#endif
