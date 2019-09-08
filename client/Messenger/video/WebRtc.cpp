#include "video/WebRtc.hpp"
// WebRTC
#include "talk/base/ssladapter.h"
#include "talk/base/win32socketinit.h"
#include "talk/base/win32socketserver.h"

void InitWebRtc() {
}

void CleanWebRtc() {
    talk_base::CleanupSSL();
}