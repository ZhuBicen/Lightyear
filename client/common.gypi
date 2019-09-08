{  
  'target_defaults': {
    'default_configuration': 'Debug',
    'configurations': {
      'Common_Base': {
        'abstract': 1,
        'msvs_configuration_attributes': {
          'OutputDirectory': '<(DEPTH)\\build\\$(ConfigurationName)',
          'IntermediateDirectory': '$(OutDir)\\obj\\$(ProjectName)',
          'CharacterSet': '1',
        },
        # Add the default import libs.
        'msvs_settings':{
          'VCLinkerTool': {
            'AdditionalDependencies': [
              'gdi32.lib',
              'Comctl32.lib',
            ],
          },
          'VCCLCompilerTool': {
            'RuntimeLibrary': '0', 
          },
        },
      },
      'Debug_Base': {
        'abstract': 1,
        'msvs_settings':{
          'VCLinkerTool': {
            'GenerateDebugInformation': 'true',
          },
          'VCCLCompilerTool': {
            'RuntimeLibrary': '1', 
          },
        },
      },
      'Debug': {
        'inherit_from': ['Common_Base', 'Debug_Base'],
        'defines': [
          'WIN32',
          '_DEBUG',
          'DEBUG',
          '_SCL_SECURE_NO_WARNINGS',
        ],
      },
      'Release': {
        'inherit_from': ['Common_Base'],
        'defines': [
          'WIN32',
          'NDEBUG',
        ],
      },
    }
  },
}
