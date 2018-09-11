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

# Common source files
set(YAMPL_COMMON_SRCS
	src/SocketFactory.cpp
	src/pipe/PipeSocketBase.cpp
	src/pipe/PipeSocketFactory.cpp
	src/shm/SHMSocketBase.cpp
	src/shm/SHMSocketFactory.cpp
	src/shm/ServerSocket.cpp
	src/zeromq/ClientSocket.cpp
	src/zeromq/ServerSocket.cpp
	src/zeromq/SocketBase.cpp
	src/zeromq/SocketFactory.cpp
)

# PThreads
find_package(Threads)

# Include YAMPL dependencies
include(cmake/YamplDepends.cmake)

# Include directories
include_directories(include/ ${CMAKE_BINARY_DIR}/config)

## @Section libyampl
add_library(yampl SHARED ${YAMPL_COMMON_SRCS})
add_dependencies(yampl CppZMQ)

target_include_directories(yampl PRIVATE ${ZEROMQ_INCLUDE_DIR} ${CPPZMQ_INCLUDE_DIR})
target_link_libraries(yampl ${ZEROMQ_SLIB_DIR}/libzmq.a rt)

## @Section Install step
set_target_properties(yampl PROPERTIES RESOURCE "${YAMPL_RESOURCES}")
    
# Config files
set(YAMPL_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/")
set(YAMPL_ENV_DIR "/etc/profile.d")

configure_file(config/yampl-env.sh.in ${CMAKE_BINARY_DIR}/scripts/yampl-env.sh @ONLY)
configure_file(config/uninstall.sh.in ${CMAKE_BINARY_DIR}/scripts/uninstall.sh @ONLY)
configure_file(config/.yamplrc.in ${CMAKE_BINARY_DIR}/scripts/.yamplrc @ONLY)
configure_file(config/yampl.pc.in ${CMAKE_BINARY_DIR}/scripts/yampl.pc @ONLY)

# Install libraries
install(TARGETS yampl
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/yampl
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/yampl
	PERMISSIONS OWNER_READ OWNER_WRITE
)

# Install public header files
install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PERMISSIONS OWNER_READ OWNER_WRITE
)

# Install .yamplrc to ~
install(FILES "${CMAKE_BINARY_DIR}/scripts/.yamplrc"
        DESTINATION $ENV{HOME}
        PERMISSIONS OWNER_READ OWNER_WRITE
)

# Install yampl.pc configuration file
install(FILES "${CMAKE_BINARY_DIR}/scripts/yampl.pc"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig
        PERMISSIONS OWNER_READ OWNER_WRITE
)
