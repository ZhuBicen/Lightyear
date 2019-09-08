#ifndef __LIGHTYEAR_MESSENGER_SIGNIN_STATECHART_HPP__
#define __LIGHTYEAR_MESSENGER_SIGNIN_STATECHART_HPP__
#include "base/StateChartBase.hpp"
#include "network/TcpSocket.hpp"
#include "MessageQueue.hpp"

namespace im {
    class RosterResp;
};

class SigninWindow;
class MainWindow;

class MainStateChart: public  IStateChartBase {
public:
    MainStateChart();
    ~MainStateChart();
    bool Init();

    void OfflineEntryAction();
    void OfflineExitAction();
    bool OfflineHandler(EventPtr);

    void SigninginEntryAction();
    void SigninginExitAction();
    bool SigninginHandler(EventPtr);

    void IdleEntryAction();
    void IdleExitAction();
    bool IdleHandler(EventPtr);

    void SignedoffEntryAction();
    void SignedoffExitAction();
    bool SignedoffHandler(EventPtr);

    void AuthenticateStateEntryAction();
    void AuthenticateStateExitAction();
    bool AuthenticateStateHandler(EventPtr);

    void RosterStateEntryAction();
    void RosterStateExitAction();
    bool RosterStateHandler(EventPtr);

    void OnlineStateEntryAction();
    void OnlineStateExitAction();
    bool OnlineStateHandler(EventPtr);

    void HeartbeatStateEntryAction();
    void HeartbeatStateExitAction();
    bool HeartbeatStateHandler(EventPtr);

    void RestartHeartbeatTimer();

    void AvailableStateEntryAction();
    void AvailableStateExitAction();
    bool AvailableStateHandler(EventPtr);

    void AwayStateEntryAction();
    void AwayStateExitAction();
    bool AwayStateHandler(EventPtr);

private:
    void CreateMainWindow();
    void CreateSigninWindow();
    void SaveRosterItems(im::RosterResp* p_message);

    StateT<MainStateChart> offline_state_;
    StateT<MainStateChart> idle_state_;
    StateT<MainStateChart> signedoff_state_;
    StateT<MainStateChart> signingin_state_;
    StateT<MainStateChart> authenticate_state_;
    StateT<MainStateChart> roster_state_;
    StateT<MainStateChart> online_state_;
    StateT<MainStateChart> heartbeat_state_;

    StateT<MainStateChart> away_state_;
    StateT<MainStateChart> available_state_;

private:
    SigninWindow* signin_window_;
    MainWindow* main_window_;
    MessageQueue message_queue_;
    TcpSocket* tcp_socket_;
};

#endif
