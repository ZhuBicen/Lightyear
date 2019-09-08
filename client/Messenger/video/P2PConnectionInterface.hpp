#ifndef LIGHTYEAR_P2PCONNECTION_INTERFACE
#define LIGHTYEAR_P2PCONNECTION_INTERFACE
// Called by the GUI thread
class P2PConnectionInterface {
 public:
  virtual void ConnectToPeer() = 0;
  virtual void DisconnectFromCurrentPeer() = 0;
  virtual void UIThreadCallback(int msg_id, void* data) = 0;
};

#endif
