#include "video/Conductor.hpp"
#include <utility>
#include "Utility.hpp"

#include "talk/base/common.h"
#include "talk/base/json.h"
#include "talk/app/webrtc/videosourceinterface.h"
#include "talk/media/devices/devicemanager.h"

#include "proto/src/core.pb.h"
#include "MessageQueue.hpp"
#include "RosterMgmt.hpp"
#include "MessageBuilder.hpp"
#include "Log.hpp"

// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

const char kAudioLabel[] = "audio_label";
const char kVideoLabel[] = "video_label";
const char kStreamLabel[] = "stream_label";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";

class DummySetSessionDescriptionObserver
    : public webrtc::SetSessionDescriptionObserver {
 public:
  static DummySetSessionDescriptionObserver* Create() {
    return
        new talk_base::RefCountedObject<DummySetSessionDescriptionObserver>();
  }
  virtual void OnSuccess() {
    MLOG(logINFO) << __FUNCTION__;
  }
  virtual void OnFailure(const std::string& error) {
    MLOG(logINFO) << __FUNCTION__ << " " << error;
  }

 protected:
  DummySetSessionDescriptionObserver() {}
  ~DummySetSessionDescriptionObserver() {}
};
Conductor::Conductor(core::Jid& jid, MessageQueue& mq, VideoWndInterface* main_wnd)
  : message_queue_(mq), peer_id_(jid), main_wnd_(main_wnd) {
}

Conductor::~Conductor() {
	DeletePeerConnection();
	ASSERT(peer_connection_.get() == NULL);
}

bool Conductor::connection_active() const {
  return peer_connection_.get() != NULL;
}

void Conductor::Close() {
  DeletePeerConnection();
}

bool Conductor::InitializePeerConnection() {
  ASSERT(peer_connection_factory_.get() == NULL);
  ASSERT(peer_connection_.get() == NULL);

  peer_connection_factory_  = webrtc::CreatePeerConnectionFactory();

  if (!peer_connection_factory_.get()) {
    // main_wnd_->MessageBox("Error",
    //    "Failed to initialize PeerConnectionFactory", true);
    DeletePeerConnection();
    return false;
  }

  webrtc::PeerConnectionInterface::IceServers servers;
  webrtc::PeerConnectionInterface::IceServer server;
  server.uri = "stun:stun.l.google.com : 19302";
  servers.push_back(server);
  peer_connection_ = peer_connection_factory_->CreatePeerConnection(servers,
                                                                    NULL,
                                                                    NULL,
                                                                    this);
  if (!peer_connection_.get()) {
    // main_wnd_->MessageBox("Error",
    //    "CreatePeerConnection failed", true);
    DeletePeerConnection();
  }
  AddStreams();
  return peer_connection_.get() != NULL;
}

void Conductor::DeletePeerConnection() {
  peer_connection_ = NULL;
  active_streams_.clear();
  main_wnd_->StopLocalRenderer();
  main_wnd_->StopRemoteRenderer();
  peer_connection_factory_ = NULL;
  peer_id_.Clear();
}

//
// PeerConnectionObserver implementation.
//

void Conductor::OnError() {
}

// Called when a remote stream is added
void Conductor::OnAddStream(webrtc::MediaStreamInterface* stream) {
    MLOG(logINFO) << __FUNCTION__;
    stream->AddRef();
    main_wnd_->QueueUIThreadCallback(NEW_STREAM_ADDED, stream);
}

void Conductor::OnRemoveStream(webrtc::MediaStreamInterface* stream) {
    MLOG(logINFO) << __FUNCTION__;
    stream->AddRef();
    main_wnd_->QueueUIThreadCallback(STREAM_REMOVED, stream);

}

void Conductor::OnIceCandidate(const webrtc::IceCandidateInterface* candidate) {
  MLOG(logINFO) << __FUNCTION__ << " " << candidate->sdp_mline_index();
  Json::StyledWriter writer;
  Json::Value jmessage;

  jmessage[kCandidateSdpMidName] = candidate->sdp_mid();
  jmessage[kCandidateSdpMlineIndexName] = candidate->sdp_mline_index();
  std::string sdp;
  if (!candidate->ToString(&sdp)) {
    MLOG(logERROR) << "Failed to serialize candidate";
    return;
  }
  jmessage[kCandidateSdpName] = sdp;
  // forward to remote peer
  core::Jid l_mySelfJid;
  l_mySelfJid.set_id(ToString(RosterMgmt::Instance()->Myself().id));
  MLOG(logINFO) << __FUNCTION__ << " ICE: " << writer.write(jmessage);
  message_queue_.PutMessage(MessageBuilder::BuildJingleMessage(l_mySelfJid, peer_id_, writer.write(jmessage)));

}

//
// PeerConnectionClientObserver implementation.
//
void Conductor::OnMessageFromPeer(const std::string& message) {
  ASSERT(peer_id_.IsInitialized());
  ASSERT(!message.empty());
  MLOG(logINFO) << " Received message from peer :" << message;

  if (!peer_connection_.get()) {

    if (!InitializePeerConnection()) {
      MLOG(logERROR) << "Failed to initialize our PeerConnection instance";
      // client_->SignOut();
      return;
    }
  }
  Json::Reader reader;
  Json::Value jmessage;
  if (!reader.parse(message, jmessage)) {
    MLOG(logWARNING) << "Received unknown message. " << message;
    return;
  }
  std::string type;
  std::string json_object;

  GetStringFromJsonObject(jmessage, kSessionDescriptionTypeName, &type);
  if (!type.empty()) {
    std::string sdp;
    if (!GetStringFromJsonObject(jmessage, kSessionDescriptionSdpName, &sdp)) {
      MLOG(logWARNING) << "Can't parse received session description message.";
      return;
    }
    webrtc::SessionDescriptionInterface* session_description(
        webrtc::CreateSessionDescription(type, sdp));
    if (!session_description) {
      MLOG(logWARNING) << "Can't parse received session description message.";
      return;
    }
    MLOG(logINFO) << __FUNCTION__ << " SetRemoteDescription";
    peer_connection_->SetRemoteDescription( DummySetSessionDescriptionObserver::Create(), session_description);
    if (session_description->type() == webrtc::SessionDescriptionInterface::kOffer) {
        MLOG(logINFO) << __FUNCTION__ << " be going to create answer";
        peer_connection_->CreateAnswer(this, NULL);
    }
    return;
  } else {
    std::string sdp_mid;
    int sdp_mlineindex = 0;
    std::string sdp;
    if (!GetStringFromJsonObject(jmessage, kCandidateSdpMidName, &sdp_mid) ||
        !GetIntFromJsonObject(jmessage, kCandidateSdpMlineIndexName,
                              &sdp_mlineindex) ||
        !GetStringFromJsonObject(jmessage, kCandidateSdpName, &sdp)) {
      MLOG(logWARNING) << "Can't parse received message.";
      return;
    }
    talk_base::scoped_ptr<webrtc::IceCandidateInterface> candidate(
        webrtc::CreateIceCandidate(sdp_mid, sdp_mlineindex, sdp));
    if (!candidate.get()) {
      MLOG(logWARNING) << "Can't parse received candidate message.";
      return;
    }
    if (!peer_connection_->AddIceCandidate(candidate.get())) {
      MLOG(logWARNING) << "Failed to apply the received candidate";
      return;
    }
    return;
  }
}

void Conductor::OnMessageSent(int err) {
}

void Conductor::OnServerConnectionFailure() {
}

//
// MainWndCallback implementation.
//
void Conductor::Connect() {
  ASSERT(peer_id_.IsInitialized());
  ASSERT(!peer_connection_.get());

  if (!InitializePeerConnection()) {
      MLOG(logERROR) << __FUNCTION__ << " Can't Initialize Peer Connection.";
	  return;
  }
  peer_connection_->CreateOffer(this, NULL);
}

void Conductor::DisconnectFromCurrentPeer() {
  MLOG(logINFO) << __FUNCTION__;
  if (peer_connection_.get()) {
	// TODO: send the HangUp message through server.
	DeletePeerConnection();
  }
}


void Conductor::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
	peer_connection_->SetLocalDescription(
		DummySetSessionDescriptionObserver::Create(), desc);
	Json::StyledWriter writer;
	Json::Value jmessage;
	jmessage[kSessionDescriptionTypeName] = desc->type();
	std::string sdp;
	desc->ToString(&sdp);
	jmessage[kSessionDescriptionSdpName] = sdp;

	// send the SDP to the remote peer, through the server.
	core::Jid l_mySelfJid;
	l_mySelfJid.set_id(ToString(RosterMgmt::Instance()->Myself().id));
    MLOG(logINFO) << __FUNCTION__ << " Message: " << writer.write(jmessage);
	message_queue_.PutMessage(MessageBuilder::BuildJingleMessage(l_mySelfJid, peer_id_, writer.write(jmessage)));
}

void Conductor::OnFailure(const std::string& error) {
    MLOG(logERROR) << __FUNCTION__ << " ERROR: " << error;
}

cricket::VideoCapturer* Conductor::OpenVideoCaptureDevice() {
    talk_base::scoped_ptr<cricket::DeviceManagerInterface> dev_manager(
        cricket::DeviceManagerFactory::Create());
    if (!dev_manager->Init()) {
        MLOG(logERROR) << "Can't create device manager";
        return NULL;
    }
    std::vector<cricket::Device> devs;
    if (!dev_manager->GetVideoCaptureDevices(&devs)) {
        MLOG(logERROR) << "Can't enumerate video devices";
        return NULL;
    }
    std::vector<cricket::Device>::iterator dev_it = devs.begin();
    cricket::VideoCapturer* capturer = NULL;
    for (; dev_it != devs.end(); ++dev_it) {
        capturer = dev_manager->CreateVideoCapturer(*dev_it);
        if (capturer != NULL)
            break;
    }
    return capturer;
}
void Conductor::AddStreams() {
    if (active_streams_.find(kStreamLabel) != active_streams_.end())
        return;  // Already added.

    talk_base::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
        peer_connection_factory_->CreateAudioTrack(
        kAudioLabel, peer_connection_factory_->CreateAudioSource(NULL)));

    talk_base::scoped_refptr<webrtc::VideoTrackInterface> video_track(
        peer_connection_factory_->CreateVideoTrack(
        kVideoLabel,
        peer_connection_factory_->CreateVideoSource(OpenVideoCaptureDevice(),
        NULL)));
    main_wnd_->StartLocalRenderer(video_track);

    talk_base::scoped_refptr<webrtc::MediaStreamInterface> stream =
        peer_connection_factory_->CreateLocalMediaStream(kStreamLabel);

    stream->AddTrack(audio_track);
    stream->AddTrack(video_track);
    if (!peer_connection_->AddStream(stream, NULL)) {
        MLOG(logERROR) << "Adding stream to PeerConnection failed";
    }
    typedef std::pair<std::string,
        talk_base::scoped_refptr<webrtc::MediaStreamInterface> >
        MediaStreamPair;
    active_streams_.insert(MediaStreamPair(stream->label(), stream));
    // main_wnd_->SwitchToStreamingUI();
}
