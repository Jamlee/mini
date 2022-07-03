{
  'variables': {
    'todep': 'todep_test', # 主动方传过来的变量无法用于 conditions
  },
  'targets': [{
    'target_name': 'hello-lib',
    'type': 'static_library',

    'sources': [
      'hello.cc',
    ],
    'direct_dependent_settings': {
      'include_dirs': [
        '.',
      ],
    },
    'conditions': [
      ['todep=="todep_test"', { # 这里有坑，condition 只能读当前gyo录变量或者-D定义的,被依赖时，主动方有这个变量也无效。
                                # 条件换成 'OS=="linux"' 就正常了。v8.gyp 也有这个毛病
        'actions': [{
          'action_name': 'hello',
          'inputs': [
            'hello.cc',
          ],
          'outputs': [
            'hello.output',
          ],
          'action': ['echo', '<(todep)'],
        }]
      }],
    ],
    # 'actions': [{
    #   'action_name': 'hello',
    #   'inputs': [
    #     'hello.cc',
    #   ],
    #   'outputs': [
    #     'hello.output',
    #   ],
    #   'action': ['echo', '<(todep)'],
    # }]
  }, ],
}
