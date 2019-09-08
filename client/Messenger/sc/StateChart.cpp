#include "StateChart.hpp"
#include "signin/SigninWindow.hpp"
#include "main/MainWindow.hpp"
#include "RosterMgmt.hpp"
#include "MessageBuilder.hpp"
#include "MessageQueue.hpp"
#include "CommandLine.hpp"
#include <tuple>
#include "Log.hpp"

MainStateChart::MainStateChart():
    IStateChartBase("SigninStateChart"),
    offline_state_(this, &MainStateChart::OfflineHandler, "OfflineState"),
    idle_state_(this, &MainStateChart::IdleHandler, "IdleState"),
    signedoff_state_(this, &MainStateChart::SignedoffHandler, "SignedOffState"),
    signingin_state_(this, &MainStateChart::SigninginHandler, "SigninginState"),
    authenticate_state_(this, &MainStateChart::AuthenticateStateHandler, "AuthenticateState"),
    roster_state_(this, &MainStateChart::RosterStateHandler, "RosterState"),
    online_state_(this, &MainStateChart::OnlineStateHandler, "OnlineState"),
    heartbeat_state_(this, &MainStateChart::HeartbeatStateHandler, "HeartbeatState"),
    available_state_(this, &MainStateChart::AvailableStateHandler, "AvailableState"),
    away_state_(this, &MainStateChart::AwayStateHandler, "AwayState"),
    message_queue_(),
    tcp_socket_(nullptr),
    main_window_(nullptr),
    signin_window_(nullptr){

    offline_state_.SetEntryAction(&MainStateChart::OfflineEntryAction);
    offline_state_.SetExitAction(&MainStateChart::OfflineExitAction);

    signingin_state_.SetEntryAction(&MainStateChart::SigninginEntryAction);
    signingin_state_.SetExitAction(&MainStateChart::SigninginExitAction);

    signedoff_state_.SetEntryAction(&MainStateChart::SignedoffEntryAction);
    signingin_state_.SetExitAction(&MainStateChart::SignedoffExitAction);

    idle_state_.SetEntryAction(&MainStateChart::IdleEntryAction);
    idle_state_.SetExitAction(&MainStateChart::IdleExitAction);

    authenticate_state_.SetEntryAction(&MainStateChart::AuthenticateStateEntryAction);
    authenticate_state_.SetExitAction(&MainStateChart::AuthenticateStateExitAction);

    roster_state_.SetEntryAction(&MainStateChart::RosterStateEntryAction);
    roster_state_.SetExitAction(&MainStateChart::RosterStateExitAction);

    online_state_.SetEntryAction(&MainStateChart::OnlineStateEntryAction);
    online_state_.SetExitAction(&MainStateChart::OnlineStateExitAction);

    heartbeat_state_.SetEntryAction(&MainStateChart::HeartbeatStateEntryAction);
    heartbeat_state_.SetExitAction(&MainStateChart::HeartbeatStateExitAction);

    available_state_.SetEntryAction(&MainStateChart::AvailableStateEntryAction);
    available_state_.SetExitAction(&MainStateChart::AvailableStateExitAction);

    away_state_.SetEntryAction(&MainStateChart::AwayStateEntryAction);
    away_state_.SetExitAction(&MainStateChart::AwayStateExitAction);

    signingin_state_.SetSuperState(&offline_state_);
    authenticate_state_.SetSuperState(&signingin_state_);
    roster_state_.SetSuperState(&signingin_state_);
    idle_state_.SetSuperState(&offline_state_);
    signedoff_state_.SetSuperState(&offline_state_);

    heartbeat_state_.SetSuperState(&online_state_);

    available_state_.SetSuperState(&heartbeat_state_);
    away_state_.SetSuperState(&heartbeat_state_);
    heartbeat_state_.SetInitTransition(&available_state_);
    offline_state_.SetInitTransition(&idle_state_);
    online_state_.SetInitTransition(&heartbeat_state_);

    SetInitialState(&offline_state_);
}

MainStateChart::~MainStateChart() {
    if (tcp_socket_ != nullptr) {
        delete tcp_socket_;
    }
}
void MainStateChart::OfflineEntryAction() {
    MLOG(logINFO) << __FUNCTION__;
    CreateSigninWindow();
    RosterMgmt::Instance()->Reset();
}

void MainStateChart::OfflineExitAction() {
    MLOG(logINFO) << __FUNCTION__;
    signin_window_->ShowWindow(false);
}

// SC can be hanged at AuthenticationSate, then ....
bool MainStateChart::OfflineHandler(EventPtr event) {
    if (event->Type() == MessageType::SigninExit) {
        PostQuitMessage(0);
        return true;
    }
    return false;
}

void MainStateChart::SigninginEntryAction() {
    MLOG(logINFO) << __FUNCTION__;
    if (tcp_socket_ != nullptr) {
        delete tcp_socket_;
    }
    tcp_socket_ = new TcpSocket(message_queue_);
    tcp_socket_->SetHwnd(*signin_window_);
}

void MainStateChart::SigninginExitAction() {
    MLOG(logINFO) << __FUNCTION__;
}

bool MainStateChart::SigninginHandler(EventPtr) {
    return false;
}
bool MainStateChart::Init() {
    return true;
}
void MainStateChart::CreateMainWindow()
{
    main_window_ = new MainWindow(message_queue_, this);
    main_window_->Create(NULL, _T("Lightyear"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_TOOLWINDOW | WS_EX_TOPMOST, 0, 0, 600, 800);
    main_window_->CenterWindow();
    tcp_socket_->SetHwnd(main_window_->GetHWND());
    ::ShowWindow(*main_window_, SW_MAXIMIZE);
}

void MainStateChart::CreateSigninWindow()
{
    signin_window_ = new SigninWindow(message_queue_, *this);
    signin_window_->Create(NULL, _T("登录"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    signin_window_->CenterWindow();
    signin_window_->ShowWindow(true);
}
void MainStateChart::IdleEntryAction() {
    MLOG(logINFO) << __FUNCTION__;
}
void MainStateChart::IdleExitAction() {
    MLOG(logINFO) << __FUNCTION__;

}

bool MainStateChart::IdleHandler(EventPtr event) {
    if (event->Type() == MessageType::StartSignin) {
        StateTransition(&authenticate_state_);
        return true;
    } else if (event->Type() == MessageType::SelfChecking) {
        auto l_autoSigninInfo = CommandLine::Instance()->GetAutoSignin();
        if (std::get<0>(l_autoSigninInfo)) {
            StateTransition(&authenticate_state_);
        }
        return true;
    }

    return false;
}
void MainStateChart::SignedoffEntryAction() {
    MLOG(logDEBUG) << __FUNCTION__;
}
void MainStateChart::SignedoffExitAction() {
    MLOG(logDEBUG) << __FUNCTION__;
}
bool MainStateChart::SignedoffHandler(EventPtr event) {
    MLOG(logDEBUG) << __FUNCTION__;
    if (event->Type() == MessageType::StartSignin) {
        StateTransition(&authenticate_state_);
        return true;
    }
    else if (event->Type() == MessageType::SigninExit) {
        PostQuitMessage(0);
        return true;
    }
    return false;

}
void MainStateChart::AuthenticateStateEntryAction() {
    MLOG(logINFO) << __FUNCTION__;
    signin_window_->m_messageQueue.PutMessage(MessageBuilder::BuildAuthenticationReq(
        signin_window_->m_accountEdit->GetText().GetData(),
        signin_window_->m_pwEdit->GetText().GetData()));
}
void MainStateChart::AuthenticateStateExitAction() {
    MLOG(logINFO) << __FUNCTION__;

}
bool MainStateChart::AuthenticateStateHandler(EventPtr event) {
    if (event->Type() == MessageType::Protobuffer) {
        auto l_proto_message = reinterpret_cast<google::protobuf::MessageLite*>(event->Data());
        if (l_proto_message->GetTypeName() == "core.AuthenticationResp") {
            auto p_message = reinterpret_cast<core::AuthenticationResp*>(l_proto_message);
            if (p_message->resp().status() == 0)
            {
                RosterMgmt::Instance()->SetMyselfId(signin_window_->m_accountEdit->GetText().GetData());
                MLOG(logDEBUG) << __FUNCTION__ << "(), Signin OKay, ResourceBinded: " << p_message->fulljid().resource();
                RosterMgmt::Instance()->SetMyselfResource(ToWstring(p_message->fulljid().resource()));
                StateTransition(&roster_state_);
                return true;
            }
            if (p_message->resp().has_cause()) {
                signin_window_->m_infoLabel->SetText(ToWstring(p_message->resp().cause()).c_str());
            }
            else {
                signin_window_->m_infoLabel->SetText(_T("服务器内部错误！"));
            }
            signin_window_->m_infoLabel->SetVisible();
            signin_window_->m_signinButton->SetEnabled();
            StateTransition(&idle_state_);
            return true;
        }
    }
    else if (event->Type() == MessageType::NetworkError) {
        signin_window_->m_infoLabel->SetText(_T("连接服务器失败！"));
        signin_window_->m_infoLabel->SetVisible();
        signin_window_->m_signinButton->SetEnabled();
        StateTransition(&idle_state_);
        return true;
    }
    MLOG(logINFO) << "Unexpected message received.";
    return false;
}




void MainStateChart::RosterStateEntryAction() {
    MLOG(logINFO) << __FUNCTION__;
    message_queue_.PutMessage(MessageBuilder::BuildRosterReq());
    // RosterMgmt::Instance()->Reset();
}
void MainStateChart::RosterStateExitAction() {
    MLOG(logINFO) << __FUNCTION__;
}
bool MainStateChart::RosterStateHandler(EventPtr event) {
    if (event->Type() == MessageType::Protobuffer) {
        auto l_proto_message = reinterpret_cast<google::protobuf::MessageLite*>(event->Data());
        if (l_proto_message->GetTypeName() == "im.RosterResp") {
            auto p_message = reinterpret_cast<im::RosterResp*>(l_proto_message);
            SaveRosterItems(p_message);

            // main_window_->OnImRosterResp(reinterpret_cast<im::RosterResp*>(l_proto_message));
            StateTransition(&online_state_);
            return true;
        }
    }
    return false;
}

void MainStateChart::OnlineStateEntryAction() {
    MLOG(logINFO) << __FUNCTION__;
    CreateMainWindow();
}

void MainStateChart::OnlineStateExitAction() {
    MLOG(logINFO) << __FUNCTION__;
    message_queue_.PutMessage(MessageBuilder::BuildPresence(im::Presence_Show_OFFLINE));
    main_window_->CloseAll();
    main_window_ = nullptr;
}

bool MainStateChart::OnlineStateHandler(EventPtr event) {
    if (event->Type() == MessageType::Protobuffer) {
        auto l_proto_message = reinterpret_cast<google::protobuf::MessageLite*>(event->Data());
        if (l_proto_message->GetTypeName() == "im.Presence") {
            main_window_->OnImPresence(reinterpret_cast<im::Presence*>(l_proto_message));
            return true;
        }
        if (l_proto_message->GetTypeName() == "im.ChatMessage") {
            main_window_->OnChatMessage(
                MessageBuilder::ToInternalChatMessage(reinterpret_cast<im::ChatMessage*>(l_proto_message)));
            return true;
        }
        if (l_proto_message->GetTypeName() == "p2p.JingleMessage") {
#ifdef ENABLE_GOOGLE_WEBRTC
            main_window_->OnP2pJingleMessage(reinterpret_cast<p2p::JingleMessage*>(l_proto_message));
            return true;
#endif
        }
    }
    if (event->Type() == MessageType::StartSignoff) {
        StateTransition(&signedoff_state_);
        return true;
    }
    if (event->Type() == MessageType::Exit) {
        main_window_->CloseAll();
        PostQuitMessage(0);
        return true;
    }
    return false;
}

void MainStateChart::HeartbeatStateEntryAction(){
    MLOG(logINFO) << __FUNCTION__;
    main_window_->StartHeartbeatTimer();
}
void MainStateChart::HeartbeatStateExitAction(){
    MLOG(logINFO) << __FUNCTION__;
    main_window_->StopHeartbeatTimer();
}
bool MainStateChart::HeartbeatStateHandler(EventPtr event){
    // if timer expired, heartbeat lost
    if (event->Type() == MessageType::Protobuffer) {
        auto l_proto_message = reinterpret_cast<google::protobuf::MessageLite*>(event->Data());
        if (l_proto_message->GetTypeName() == "core.Heartbeat") {
            message_queue_.PutMessage(MessageBuilder::BuildHeartbeatAck());
            RestartHeartbeatTimer();
            return true;
        }
    } 
    else if ( event->Type() == MessageType::HearbeatTimeout) {
        // TODO, reconnecting
        MLOG(logDEBUG) << __FUNCTION__ << " HeartbeatTimeout";
        StateTransition(&idle_state_);
        return true;
    }
    return false;
}

void MainStateChart::AvailableStateEntryAction(){
    message_queue_.PutMessage(MessageBuilder::BuildPresence(im::Presence_Show_ONLINE));
    MLOG(logDEBUG) << __FUNCTION__;
}
void MainStateChart::AvailableStateExitAction(){
    MLOG(logDEBUG) << __FUNCTION__;
}
bool MainStateChart::AvailableStateHandler(EventPtr event){
    if (event->Type() == MessageType::ChangeToAway) {
        MLOG(logDEBUG) << __FUNCTION__ << ": ChangeToAway";
        StateTransition(&away_state_);
        return true;
    }
    return false;
}
void MainStateChart::AwayStateEntryAction() {
    MLOG(logDEBUG) << __FUNCTION__;
    message_queue_.PutMessage(MessageBuilder::BuildPresence(im::Presence_Show_AWAY));
}
void MainStateChart::AwayStateExitAction(){
    MLOG(logDEBUG) << __FUNCTION__;
}
bool MainStateChart::AwayStateHandler(EventPtr event) {
    if (event->Type() == MessageType::ChangeToAvailable) {
        MLOG(logDEBUG) << __FUNCTION__ << ": ChangeToAvailable";
        StateTransition(&available_state_);
        return true;
    }
    return false;
}
void MainStateChart::SaveRosterItems(im::RosterResp* p_message)
{
    for (auto l_i = 0; l_i < p_message->rosteritems_size(); ++l_i) {
        auto l_rosterMsgItem = p_message->rosteritems(l_i);
        RosterItem l_rosterItem;
        l_rosterItem.id = ToWstring(l_rosterMsgItem.jid().id()).c_str();
        // TODO: logo
        l_rosterItem.logo = _T("default.png");
        // TODO: remove the magic number 
        if (l_rosterMsgItem.onlinestatus() == 1) {
            l_rosterItem.status = im::Presence_Show_ONLINE;
            MLOG(logINFO) << " Found online friend:" << l_rosterMsgItem.jid().id();
        }
        else {
            l_rosterItem.status = im::Presence_Show_OFFLINE;
        }

        l_rosterItem.name = boost::optional<std::wstring>(l_rosterMsgItem.has_name(), 
            ToWstring(l_rosterMsgItem.name()));
        l_rosterItem.position = boost::optional<std::wstring>(l_rosterMsgItem.has_position(), 
            ToWstring(l_rosterMsgItem.position()));
        l_rosterItem.department = boost::optional<std::wstring>(l_rosterMsgItem.has_department(), 
            ToWstring(l_rosterMsgItem.department()));


        RosterMgmt::Instance()->AddRosterItem(l_rosterItem);
    }
    RosterMgmt::Instance()->Sort();
}

void MainStateChart::RestartHeartbeatTimer()
{
    main_window_->StopHeartbeatTimer();
    main_window_->StartHeartbeatTimer();
}

