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

# Preamble
cmake_minimum_required(VERSION 2.8.10)
project(yampl)

# Use C++14
include(cmake/YamplUtils.cmake)
USE_CXX14()

# Common source files for plugins
set(YAMPL_PLUGIN_COMMON_SRCS src/ISocket.cpp src/plugin/PluginArbiter.cpp)

# Common libraries (for examples and tests)
set(YAMPL_COMMON_LIBS ${CMAKE_BINARY_DIR}/libyampl.so dl uuid pthread)

# PThreads
find_package(Threads)

# Include YAMPL dependencies
include(cmake/YamplDepends.cmake)

# Include directories
include_directories(
        include/
        /usr/local/include
)

# Library directiories
link_directories(/usr/local/lib)

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

if(WITH_EXAMPLES)
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

 if(WITH_TESTS)
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
file(GLOB_RECURSE YAMPL_PUBLIC_HEADER "include/yampl/*.h" "include/yampl/*.hpp")

set_target_properties(yampl PROPERTIES PUBLIC_HEADER "${YAMPL_PUBLIC_HEADER}")
set_target_properties(yampl PROPERTIES RESOURCE "${YAMPL_RESOURCES}")

# Configure yampl-env.sh.in
set(YAMPL_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/lib/yampl")
set(YAMPL_ENV_DIR "/etc/profile.d")

configure_file(config/yampl-env.sh.in ${CMAKE_BINARY_DIR}/scripts/yampl-env.sh @ONLY)
configure_file(config/uninstall.sh.in ${CMAKE_BINARY_DIR}/scripts/uninstall.sh @ONLY)

# @todo: Generate project configuration file
install(TARGETS yampl
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/yampl
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/yampl
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/static/yampl
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/yampl        
)

# Unpack resources
install(DIRECTORY ${CMAKE_BINARY_DIR}/scripts/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/yampl/rsrc
        PATTERN "scripts/*.sh"
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ
)

# Copy yampl-env.sh to /etc/profile.d
install(FILES ${CMAKE_BINARY_DIR}/scripts/yampl-env.sh
        DESTINATION ${YAMPL_ENV_DIR}
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ
)

# Include plugins
#execute_process(COMMAND sh "-c" "rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/plugins; mkdir ${CMAKE_CURRENT_SOURCE_DIR}/plugins")

execute_process(COMMAND mkdir ${CMAKE_BINARY_DIR}/plugins)
if (WITH_ZMQ_PLUGIN)
        AddExtProjectGit("https://github.com/ntauth/yampl-zmq" "${CMAKE_BINARY_DIR}/plugins")        
endif()

file(GLOB YAMPL_PLUGIN_CMAKE_FILES plugins/**/*.cmake ${CMAKE_BINARY_DIR}/plugins/**/*.cmake)
foreach(YAMPL_CMAKE_FILE ${YAMPL_PLUGIN_CMAKE_FILES})
    include(${YAMPL_CMAKE_FILE})
endforeach(YAMPL_CMAKE_FILE)
