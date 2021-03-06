// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "CefAppImpl.hpp"
#include "MessageHistorySchemeHandler.hpp"
#include <string>

CefAppImpl::CefAppImpl() {
}
void CefAppImpl::OnContextInitialized() {
    CefRegisterSchemeHandlerFactory("lightyear", "messagehistory", new MessageHistorySchemeHandlerFactory());
}

void CefAppImpl::OnRegisterCustomSchemes(
    CefRefPtr<CefSchemeRegistrar> registrar) {
    registrar->AddCustomScheme("lightyear", true, false, false);
}
