{
  'variables': {
    'todep': 'todep_test', # 引入时如被依赖的gyp可以拿到这个变量
  },
  'targets': [{
    'target_name': 'v8-demo',
    'type': 'executable',
    'include_dirs': [], # C++ include 目录
    'dependencies': [
      'tools/hello_gypfiles/hello.gyp:hello-lib',
      'tools/v8_gypfiles/v8.gyp:v8_maybe_snapshot',
      'tools/v8_gypfiles/v8.gyp:v8_libplatform',
    ],
    'sources': [
      'src/main.cc',
      'src/hi.cc',
      'src/hi.h',
      'src/call_js.cc',
      'src/call_js.h',
    ]
  }, ],
}
