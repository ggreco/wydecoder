{
  "targets": [
    {
      "target_name": "wydecoder",
      "cflags!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags_cc!": [ "-fno-exceptions", "-fno-rtti" ],
      "cflags": [ "-std=c++14 -O3" ],
      "sources": [
        "decoder/decoder_base.cpp",
        "decoder/simple_decoder.cpp",
        "decoder/reverse_decoder.cpp",
        "utils/thread.cpp",
        "utils/logger.cpp",
        "module.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "decoder",
        "utils",
        "ext_inc"
      ],
      'defines': [ ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'MACOSX_DEPLOYMENT_TARGET': '10.9',
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_ENABLE_CPP_RTTI': 'YES'
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
          ],
          'include_dirs': [ "ext_inc/SDL2", "ext_inc_mac" ]
        }],
        ['OS=="linux"', {
          'cflags': [ "-fPIC" ],
          'ldflags': [ "-Wl,-Bsymbolic" ],
          'libraries' : [
              '../extlib_linux/libavformat.a',
              '../extlib_linux/libavcodec.a',
              '../extlib_linux/libavutil.a',
              '../extlib_linux/libswresample.a',
              '../extlib_linux/libswscale.a',
              '../extlib_linux/libx264.a',
              '../extlib_linux/libfdk-aac.a',
              '-lSDL2', '-lz', '-lbz2', '-llzma', 
              '-lssl', '-lcrypto', '-lrt', '-ldl'
          ],
          'include_dirs': [ "/usr/include/SDL2" ]
        }],
        ['OS=="win"', {
          'include_dirs': [ "ext_inc/SDL2", "ext_inc_win" ],
          'libraries' : [
              '../extlib_w64/libavformat.a',
              '../extlib_w64/libavcodec.a',
              '../extlib_w64/libavutil.a',
              '../extlib_w64/libswresample.a',
              '../extlib_w64/libswscale.a',
              '../extlib_w64/libSDL2.a',
              '../extlib_w64/libx264.a',
              '../extlib_w64/libfdk-aac.a',
              '-lz', '-lbz2'
          ]
        }]
      ]
    }
  ]
}
