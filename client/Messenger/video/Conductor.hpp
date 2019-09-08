#ifndef PEERCONNECTION_SAMPLES_CLIENT_CONDUCTOR_H_
#define PEERCONNECTION_SAMPLES_CLIENT_CONDUCTOR_H_
#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>
#include "proto/src/core.pb.h"
#include "VideoWndInterface.hpp"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "talk/base/scoped_ptr.h"

class MessageQueue;

namespace webrtc {
class VideoCaptureModule;
}  // namespace webrtc

namespace cricket {
class VideoRenderer;
}  // namespace cricket

class Conductor
  : public webrtc::PeerConnectionObserver,
    public webrtc::CreateSessionDescriptionObserver {
 public:
  enum CallbackID {
    MEDIA_CHANNELS_INITIALIZED = 1,
    PEER_CONNECTION_CLOSED,
    SEND_MESSAGE_TO_PEER,
    PEER_CONNECTION_ERROR,
    NEW_STREAM_ADDED,
    STREAM_REMOVED,
  };

  // remote peer's Jid
  Conductor(core::Jid& jid, MessageQueue& mq, VideoWndInterface* main_wnd);

  bool connection_active() const;

  virtual void Close();

 protected:
  ~Conductor();
  bool InitializePeerConnection();
  void DeletePeerConnection();
  //
  // PeerConnectionObserver implementation.
  //
  virtual void OnError();
  virtual void OnStateChange(
      webrtc::PeerConnectionObserver::StateType state_changed) {}
  virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
  virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);
  virtual void OnRenegotiationNeeded() {}
  virtual void OnIceChange() {}
  virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);

public:
  // To process the message sent from server, eg SDP.
  virtual void OnMessageFromPeer(const std::string& message);
  virtual void OnMessageSent(int err);
  virtual void OnServerConnectionFailure();

public:
  //
  // P2PConnectionInterface implementation.
  //
  virtual void Connect();
  virtual void DisconnectFromCurrentPeer();
  virtual void UIThreadCallback(int msg_id, void* data){}

protected:
  // CreateSessionDescriptionObserver implementation.
  virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
  virtual void OnFailure(const std::string& error);


private:
    void AddStreams();
    cricket::VideoCapturer* OpenVideoCaptureDevice();
private:
  core::Jid peer_id_;
  talk_base::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
  talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface>
      peer_connection_factory_;
  std::deque<std::string*> pending_messages_;
  std::map<std::string, talk_base::scoped_refptr<webrtc::MediaStreamInterface> >
      active_streams_;
  std::string server_;

  // Put message to the Queue which will be sent to remote peer through server
  MessageQueue& message_queue_;

  //
  VideoWndInterface* main_wnd_;
};

#endif  // PEERCONNECTION_SAMPLES_CLIENT_CONDUCTOR_H_
