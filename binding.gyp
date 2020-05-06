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
      "library_dirs": [
        "<(module_root_dir)/extlib_<!@(node -e \"console.log('%s/%s',require('process').platform,require('process').arch);\")"
      ],
      "libraries": [  
        '-lavcodec', '-lavformat', '-lavutil', '-lswscale', '-lswresample', '-lSDL2'
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
              '-lfdk-aac', '-lx264', '-lz', '-lbz2',
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
              '-lfdk-aac', '-lx264', '-lz', '-lbz2',
              '-lssl', '-lcrypto', '-lrt', '-ldl'
          ],
          'include_dirs': [ "/usr/include/SDL2" ]
        }],
        ['OS=="win"', {
          'include_dirs': [ "ext_inc/SDL2", "ext_inc_win" ],
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
