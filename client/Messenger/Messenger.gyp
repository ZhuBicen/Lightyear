{
  'variables': {
    'cef_binary': [
        'cef_binary_3.2217.1922_windows32',
    ],
  },
  'includes': [
    '../common.gypi',
    # why is the following not expanded?
    #'../<(cef_binary)/cef_paths2.gypi',
    #'../cef_binary_3.2078.1780_windows32/cef_paths2.gypi',
  ],
  'targets': [
    {
      'target_name': 'Messenger',
      'type': 'executable',
      'include_dirs': [
        '.',
        'D:/boost_1_57_0',
        '../<(cef_binary)',
        '../../tools/protobuf-2.6.1/src',
      ],
      'defines': [
        'WIN32_LEAN_AND_MEAN',
        #'ENABLE_GOOGLE_WEBRTC',
      ],
      'defines!': [
        '_HAS_EXCEPTIONS=0',
        'NOMINMAX',
      ],
      'library_dirs': [
        'D:/boost_1_57_0/stage/lib',
        '../<(cef_binary)/$(ConfigurationName)',
      ],
      'msvs_disabled_warnings': [
          4541, #'typeid' used on polymorphic type 'boost::locale::util::base_converter' with /GR-; unpredictable behavior may result
      ],
      'dependencies': [
        '../GenerateVersion/GenerateVersion.gyp:GenerateVersion',
        '../DuiLib/DuiLib.gyp:DuiLib',
      ],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'ExceptionHandling': '1',  # /EHsc
        },
        'VCLinkerTool': {
          'SubSystem': '2',  # Windows
        },
      },
      'link_settings':{
        'libraries':[
          'Comctl32.lib',
          '-l../<(cef_binary)/$(ConfigurationName)/libcef.lib',
          '-l../<(cef_binary)/out/$(ConfigurationName)/lib/libcef_dll_wrapper.lib',
          '-l../../tools/protobuf-2.6.1/vsprojects/$(ConfigurationName)/libprotobuf.lib',
        ],
      },
      # 'copies': [
        # {
            # 'destination': '$(OutDir)',
            # 'files': [
                # 'skin/emotion/',
                # 'skin/css/',
            # ],
        # },
      # ],
      'sources': [
        'base/StateChartBase.hpp',
        'base/StateChartBase.cpp',
        'about/AboutWindow.hpp',
        'about/AboutWindow.cpp',
        'control/RosterList.cpp',
        'control/RosterList.hpp',
        'control/TrayIcon.cpp',
        'control/TrayIcon.hpp',
        'control/TrayMenu.hpp',
        'control/TrayMenu.cpp',
        'control/RosterItemUI.hpp',
        'control/RosterItemUI.cpp',
        'control/RosterLogoButton.cpp',
        'control/RosterLogoButton.hpp',
        'control/RecentRosterList.cpp',
        'control/RecentRosterList.hpp',
        'control/LightyearMenu.hpp',
        'control/RosterContextMenu.hpp',
        'control/RosterContextMenu.cpp',

        'network/TcpSocket.cpp',
        'network/TcpSocket.hpp',

        'chat/ChatWindow.cpp',
        'chat/ChatWindow.hpp',
        'chat/ChatMessage.cpp',
        'chat/ChatMessage.hpp',
        'chat/ScreenshotWindow.hpp',
        'chat/ScreenshotWindow.cpp',
        'chat/SmileyWindow.hpp',
        'chat/SmileyWindow.cpp',

        'main/MainWindow.hpp',
        'main/MainWindow.cpp',
        'signin/SigninWindow.hpp',
        'signin/SigninWindow.cpp',

        'sc/StateChart.hpp',
        'sc/StateChart.cpp',
        'sc/StateChart.plantuml',

        'window/LightyearWindowBase.hpp',
        'window/LightyearWindowBase.cpp',

        #'video/VideoWindow.hpp',
        #'video/VideoWindow.cpp',
        #'video/Conductor.hpp',
        #'video/Conductor.cpp',
        #'video/VideoWndInterface.hpp',
        #'video/P2PConnectionInterface.hpp',
        #'video/WebRtc.hpp',
        #'video/WebRtc.cpp',
        #'video/PeerConnectionObserver.hpp',
        'cef/CefAppImpl.cpp',
        'cef/CefAppImpl.hpp',
        'cef/CefBrowserWrapper.cpp',
        'cef/CefBrowserWrapper.hpp', 
        'cef/CefClientImpl.cpp',
        'cef/CefClientImpl.hpp',
        'cef/MessageHistorySchemeHandler.hpp',
        'cef/MessageHistorySchemeHandler.cpp',

        'CommandLine.cpp',
        'CommandLine.hpp',
        'DrawUtil.hpp',
        'Event.hpp',
        'Event.cpp',
        'ImageDataObject.cpp',
        'ImageDataObject.hpp',
        'MessageBuilder.cpp',
        'MessageBuilder.hpp',
        'MessageQueue.hpp',
        'Messenger.gyp',
        'Messenger.rc',
        'RosterMgmt.cpp',
        'RosterMgmt.hpp',
        'LocalProfile.hpp',
        'LocalProfile.cpp',
        'UiMessage.hpp',
        'Log.hpp',
        'main.cpp',
        'resource.h',
        'Utility.hpp',
        'Utility.cpp',
        'Version.h',
        'Version.ver',

        'proto/Codec.hpp',
        'proto/Codec.cpp',
        'proto/src/core.pb.cc',
        'proto/src/core.pb.h',
        'proto/src/im.pb.cc',
        'proto/src/im.pb.h',
        'proto/src/localdb.pb.cc',
        'proto/src/localdb.pb.h',
        'proto/src/p2p.pb.h',
        'proto/src/p2p.pb.cc',

      ],
    },
 ],
 }
