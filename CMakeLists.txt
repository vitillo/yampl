# @Project YAMPL (Yet Another Message Passing Library)
message("
     ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄       ▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄
    ▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░▌     ▐░░▌▐░░░░░░░░░░░▌▐░▌
    ▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀█░▌▐░▌░▌   ▐░▐░▌▐░█▀▀▀▀▀▀▀█░▌▐░▌
    ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌▐░▌ ▐░▌▐░▌▐░▌       ▐░▌▐░▌
    ▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌ ▐░▐░▌ ▐░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌
    ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌  ▐░▌  ▐░▌▐░░░░░░░░░░░▌▐░▌
     ▀▀▀▀█░█▀▀▀▀ ▐░█▀▀▀▀▀▀▀█░▌▐░▌   ▀   ▐░▌▐░█▀▀▀▀▀▀▀▀▀ ▐░▌
         ▐░▌     ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌          ▐░▌
         ▐░▌     ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌          ▐░█▄▄▄▄▄▄▄▄▄
         ▐░▌     ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌          ▐░░░░░░░░░░░▌
          ▀       ▀         ▀  ▀         ▀  ▀            ▀▀▀▀▀▀▀▀▀▀▀
          Yet Another Message Passing Library
")
message(STATUS "Configuring...")

cmake_minimum_required(VERSION 2.8.10)

## @Section Libyampl
project(yampl)
set(YAMPL_VERSION 1.0)

# Use C++14
include(cmake/YamplUtils.cmake)
USE_CXX14()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-terminate -Wno-return-type -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DNDEBUG)
endif()

# Default install prefix
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX $ENV{HOME}/yampl CACHE PATH "" FORCE)
endif()

if (NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# PThreads
find_package(Threads)

# Include YAMPL dependencies
include(cmake/YamplDepends.cmake)

# Include directories
include_directories(include/ ${CMAKE_BINARY_DIR}/config)

## @Section Install step
# Install public header files
install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PERMISSIONS OWNER_READ OWNER_WRITE
)
