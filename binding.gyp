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
      "libraries": [  
        '-lavcodec', '-lavformat', '-lavutil', '-lswscale', '-lswresample', '-lSDL2', '-lfdk-aac', '-lx264', '-lz', '-lbz2'
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
              '-L../extlib_osx',
              '-framework Cocoa',
              '-framework CoreAudio',
              '-framework AudioToolbox'
          ],
          'include_dirs': [ "ext_inc/SDL2", "ext_inc_mac" ]
        }],
        ['OS=="linux"', {
          'cflags': [ "-fPIC" ],
          'ldflags': [ "-Wl,-Bsymbolic" ],
          'libraries' : [
              "<!@(node -e \"console.log('-L../extlib_linux/%s',require('process').arch);\")"
              '-lssl', '-lcrypto', '-lrt', '-ldl'
          ],
          'include_dirs': [ "/usr/include/SDL2" ]
        }],
        ['OS=="win"', {
          'include_dirs': [ "ext_inc/SDL2", "ext_inc_win" ],
          'libraries' : [
              "<!@(node -e \"console.log('-L../extlib_win/%s',require('process').arch);\")",
              '-lz', '-lbz2'
          ],
          "msvs_settings": {
              "VCCLCompilerTool": {
                "ExceptionHandling": "2"
              }
          }
        }]
      ]
    }
  ]
}
