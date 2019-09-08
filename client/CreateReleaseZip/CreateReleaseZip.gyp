{
  'targets': [    
    {
      'target_name': 'CreateReleaseZip',
      'type': 'none',
      'dependencies': [
        '../Messenger/Messenger.gyp:*',
      ],
      'actions': [
        {
          'inputs': [ 
            'CreateReleaseZip.py',
        ],
          'outputs': [
            './dummy.txt.always',
          ],
          'action_name': 'Create Release Zip',
          'action':
           ['python', './CreateReleaseZip.py', '$(OutDir)'],
        },
      ],
      'msvs_cygwin_shell': 0,
    },
  ],
}