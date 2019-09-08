{
  'targets': [    
	{
      'target_name': 'GenerateVersion',
      'type': 'none',
      'actions': [
        {
          'inputs': [ 
		     'generate_version.py',
		   ],
          'outputs': [
		    './dummy.txt.always',
            '../Messenger/Version.ver',
          ],
          'action_name': 'generate header',
          'action':
           ['python', './generate_version.py', '../Messenger/version.ver', '$(OutDir)'],
        },
      ],
      'msvs_cygwin_shell': 0,
    },
  ],
}