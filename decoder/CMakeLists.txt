# create the library
add_library(decoder decoder_base.cpp  decoder_base.h  reverse_decoder.cpp  reverse_decoder.h  simple_decoder.cpp  simple_decoder.h)

# target the libs & headers
target_link_libraries(decoder utils ${FFMPEG_LIBRARIES})
target_include_directories(decoder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${FFMPEG_INCLUDE_DIRS})

