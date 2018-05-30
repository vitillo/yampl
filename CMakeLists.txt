##
# @author: Ayoub Chouak (a.chouak@protonmail.com)
# @brief:  CMakeLists for YAMPL
##
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
project(yampl)

# Use C++14
include(cmake/YamplUtils.cmake)
USE_CXX14()

# Default install prefix
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ~/yampl)
endif()

# Common source files for plugins
set(YAMPL_PLUGIN_COMMON_SRCS src/ISocket.cpp src/plugin/PluginArbiter.cpp)

# Common libraries (for examples and tests)
set(YAMPL_COMMON_LIBS ${CMAKE_BINARY_DIR}/libyampl.so dl uuid pthread)

# PThreads
find_package(Threads)

# Include YAMPL dependencies
include(cmake/YamplDepends.cmake)

# Include directories
include_directories(include/ /usr/local/include)

# Source files
add_library(yampl SHARED
        src/plugin/PluginArbiter.cpp
        src/plugin/DynamicModule.cpp
        src/utils/utils.cpp
        src/SocketFactory.cpp
        src/ISocket.cpp
)

# Suppress -Wterminate
target_compile_options(yampl PRIVATE "-Wno-terminate")

option(WITH_EXAMPLES "Include examples" ON)

if(WITH_EXAMPLES)
    # Dummy Plugin
    add_library(yampl-dummy SHARED examples/dummy_plugin/plugin_main.cpp)
    add_executable(consumer examples/dummy_plugin/consumer.cpp)
    set_target_properties(yampl-dummy
            PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/dummy_plugin/lib"
    )
    set_target_properties(consumer
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/dummy_plugin/bin"
    )
    target_link_libraries(yampl-dummy ${YAMPL_COMMON_LIBS})
    target_link_libraries(consumer ${YAMPL_COMMON_LIBS})
    add_dependencies(yampl-dummy yampl)
    add_dependencies(consumer yampl)

    # Client-Server
    add_executable(client examples/client_server/client.cpp)
    add_executable(server examples/client_server/server.cpp)
    set_target_properties(client server
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/client_server/bin"
    )
    target_link_libraries(client ${YAMPL_COMMON_LIBS})
    target_link_libraries(server ${YAMPL_COMMON_LIBS})
    add_dependencies(client yampl)
    add_dependencies(server yampl)
    target_compile_options(client PRIVATE "-Wno-terminate")
    target_compile_options(server PRIVATE "-Wno-terminate")
    
    # Benchmarks
    add_executable(benchmark examples/benchmarks/benchmark.cpp)
    set_target_properties(benchmark
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/benchmark/bin"
    )
    target_link_libraries(benchmark ${YAMPL_COMMON_LIBS})
    add_dependencies(benchmark yampl)
    target_compile_options(benchmark PRIVATE "-Wno-terminate")
endif()

option(WITH_TESTS "Include tests" ON)

if(WITH_TESTS)
    # PluginArbiter Test
    add_executable(pluginarbiter tests/pluginarbiter.cpp)
    set_target_properties(pluginarbiter
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/"
            )
    target_link_libraries(pluginarbiter ${YAMPL_COMMON_LIBS})
    add_dependencies(pluginarbiter yampl)

    # Dynmodule Test
    add_executable(dynmodule tests/dynmodule.cpp)
    set_target_properties(dynmodule
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/"
    )
    target_link_libraries(dynmodule ${YAMPL_COMMON_LIBS})
    add_dependencies(dynmodule yampl)

    # Calls Test
    add_executable(calls tests/calls.cpp)
    set_target_properties(calls
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/"
    )
    target_link_libraries(calls ${YAMPL_COMMON_LIBS})
    add_dependencies(calls yampl)

    # Dest Test
    add_executable(dest tests/dest.cpp)
    set_target_properties(dest
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/"
    )
    target_link_libraries(dest ${YAMPL_COMMON_LIBS})
    add_dependencies(dest yampl)

    # Size Test
    add_executable(size tests/size.cpp)
    set_target_properties(size
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/"
    )
    target_link_libraries(size ${YAMPL_COMMON_LIBS})
    add_dependencies(size yampl)
endif()

# Libraries
target_link_libraries(yampl
    ${CMAKE_THREAD_LIBS_INIT}
    rt
    uuid
)

# Install step
file(GLOB_RECURSE YAMPL_PUBLIC_HEADER "include/*.h" "include/*.hpp" "include/yampl/*.h" "include/yampl/*.hpp")

set_target_properties(yampl PROPERTIES PUBLIC_HEADER "${YAMPL_PUBLIC_HEADER}")
set_target_properties(yampl PROPERTIES RESOURCE "${YAMPL_RESOURCES}")

# Configure yampl-env.sh.in
set(YAMPL_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/lib")
set(YAMPL_ENV_DIR "/etc/profile.d")

configure_file(config/yampl-env.sh.in ${CMAKE_BINARY_DIR}/scripts/yampl-env.sh @ONLY)
configure_file(config/uninstall.sh.in ${CMAKE_BINARY_DIR}/scripts/uninstall.sh @ONLY)
configure_file(config/.yamplrc.in ${CMAKE_BINARY_DIR}/scripts/.yamplrc @ONLY)
configure_file(config/yampl.pc.in ${CMAKE_BINARY_DIR}/scripts/yampl.pc @ONLY)

install(TARGETS yampl
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/yampl
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/yampl
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/yampl     
        PERMISSIONS WORLD_READ OWNER_WRITE
)

# Install scripts
install(FILES "${CMAKE_BINARY_DIR}/scripts/uninstall.sh"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/scripts/yampl-uninstall
        PERMISSIONS WORLD_READ OWNER_WRITE OWNER_EXECUTE
)

# Install .yamplrc to ~
install(FILES "${CMAKE_BINARY_DIR}/scripts/.yamplrc"
        DESTINATION ~/
        PERMISSIONS WORLD_READ OWNER_WRITE
)

# Install yampl.pc configuration file
install(FILES "${CMAKE_BINARY_DIR}/scripts/yampl.pc"
        DESTINATION /usr/lib/pkgconfig
        PERMISSIONS WORLD_READ OWNER_WRITE
)

# Make a directory for custom plugins
execute_process(COMMAND mkdir ${CMAKE_BINARY_DIR}/plugins)

# WITH_PLUGIN_{SHM,PIPE}=ON default
option(WITH_PLUGIN_SHM "Include yampl-shm" ON)
option(WITH_PLUGIN_PIPE "Include yampl-pipe" ON)

if (WITH_PLUGIN_ZMQ)
    AddExtProjectGit("https://github.com/ntauth/yampl-zmq" "${CMAKE_BINARY_DIR}/plugins")        
endif()

# Include plugins
file(GLOB_RECURSE YAMPL_PLUGIN_CMAKE_FILES "plugins/*.conf.cmake" "${CMAKE_BINARY_DIR}/plugins/*.conf.cmake")
foreach(YAMPL_CMAKE_FILE ${YAMPL_PLUGIN_CMAKE_FILES})
    string(REGEX REPLACE "(\\.conf\\.cmake)$" "" YAMPL_PLUGIN_NAME ${YAMPL_CMAKE_FILE})
    include(${YAMPL_CMAKE_FILE})
endforeach(YAMPL_CMAKE_FILE)

# Copy the install_manifest to scripts/yampl-uninstall
install(FILES "${CMAKE_BINARY_DIR}/install_manifest.txt"
        DESTINATION ${CMAKE_INSTALL_PREFIX}/scripts/yampl-uninstall
        PERMISSIONS WORLD_READ OWNER_WRITE
)
