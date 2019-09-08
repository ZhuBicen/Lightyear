# Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.

{
  'includes': [
    'common.gypi',
  ],
  'variables': {
    'client_root%': '<(DEPTH)',
  },
  'targets': [
    {
      'target_name': 'all',
      'type': 'none',
      'dependencies': [
        'Messenger/Messenger.gyp:Messenger',
        'CreateReleaseZip/CreateReleaseZip.gyp:CreateReleaseZip',
    ],
    },
  ]
}
