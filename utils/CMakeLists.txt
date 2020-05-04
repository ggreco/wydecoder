if (APPLE)
    set(PLATFORM_SOURCES osxstuff.mm)
endif()

add_library(utils logger.cpp token.cpp uri_utils.cpp thread.cpp ${PLATFORM_SOURCES})
