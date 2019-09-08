{
  'includes': [
    '../../talk/build/common.gypi',
  ],
  'targets': [
    {
      'target_name': 'VideoWindowTest',
      'type': 'executable',
      'dependencies': [
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
        ],
      },
      'sources': [
        'VideoWindow.hpp',
        'VideoWindow.cpp',
        'main.cpp',
        'skin/xml/VideoWindow.xml',
      ],
    },
 ],
 }
