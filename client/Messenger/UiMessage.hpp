#ifndef LIGHTYEAR_MESSENGER_UI_MESSAGE_HPP
#define LIGHTYEAR_MESSENGER_UI_MESSAGE_HPP

#include <windows.h>

const UINT WM_SCREENSHOT = WM_USER + __LINE__;

const UINT WM_INTERNAL_MESSAGE = WM_USER + __LINE__;
const UINT WM_MENU_MESSAGE = WM_USER + __LINE__;
const UINT WM_CEF_LOADED = WM_USER + __LINE__;
const UINT WM_CEF_LOADING = WM_USER + __LINE__;

// communicating with state chart
enum class MessageType {
    Protobuffer = 0,
    StartSignin,
    StartSignoff,
    SigninExit,
    SelfChecking,
    NetworkError,
    StreamAdded,
    StreamRemoved,
    RenderVideo,

    HearbeatTimeout,

    ChangeToAvailable,
    ChangeToAway,
    Exit,

};

enum class MenuItem {
    StartChat,
    StartVideo,
    ShowMainWindow,
    ExitMainWindow,
};
const UINT WM_TIMER_CEF_LOADING = 102;
const UINT WM_USER_END = WM_USER + __LINE__;
#endif
