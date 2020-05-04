{
  "targets": [
    {
      "target_name": "wydecoder",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "cflags": [ "-std=c++14 -O3" ],
      "sources": [
        "decoder/decoder_base.cpp",
        "decoder/simple_decoder.cpp",
        "utils/thread.cpp",
        "utils/logger.cpp",
        "module.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "decoder",
        "utils",
        "ext_inc/SDL2"
      ],
      'defines': [ ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          },
          'libraries' : [
              '../extlib_osx/libavformat.a',
              '../extlib_osx/libavcodec.a',
              '../extlib_osx/libavutil.a',
              '../extlib_osx/libswresample.a',
              '../extlib_osx/libswscale.a',
              '../extlib_osx/libSDL2.a',
              '../extlib_osx/libx264.a',
              '../extlib_osx/libfdk-aac.a',
              '-framework Cocoa',
              '-framework CoreAudio',
              '-framework AudioToolbox',
              '-lz', '-lbz2', '-llzma'
          ]
        }]
      ]
    }
  ]
}