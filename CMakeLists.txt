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

# Pthreads
find_package(Threads)

# Include YAMPL dependencies
include(cmake/YamplDepends.cmake)

# Include directories
include_directories(include/ /usr/local/include)

# Library directiories
# link_directories(/usr/local/lib)

# Source files
add_library(yampl SHARED
        src/plugin/PluginArbiter.cpp
        src/plugin/DynamicModule.cpp
        src/utils/utils.cpp
        src/SocketFactory.cpp
        src/ISocket.cpp
)

if(WITH_EXAMPLES STREQUAL ON)
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
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/client_server/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/client_server/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/client_server/bin"
    )
    target_link_libraries(client ${YAMPL_COMMON_LIBS})
    target_link_libraries(server ${YAMPL_COMMON_LIBS})
    add_dependencies(client yampl)
    add_dependencies(server yampl)
	
    # Benchmarks
    add_executable(benchmark examples/benchmarks/benchmark.cpp)
    set_target_properties(benchmark
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/benchmark/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/benchmark/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/benchmark/bin"
    )
    target_link_libraries(benchmark ${YAMPL_COMMON_LIBS})
    add_dependencies(benchmark yampl)
endif()

if(WITH_TESTS STREQUAL ON)
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
target_link_libraries(yampl ${CMAKE_THREAD_LIBS_INIT}
    rt
    uuid
    libuv
    scy_base
    scy_pluga
    zlib
    minizip
    http_parser
)

# Properties
file(GLOB_RECURSE YAMPL_PUBLIC_HEADER "include/yampl/*.h" "include/yampl/*.hpp")
set_target_properties(yampl PROPERTIES PUBLIC_HEADER "${YAMPL_PUBLIC_HEADER}")

# Add dependencies
add_dependencies(yampl LibSourcey)

# Install step
# @todo: Generate project configuration file
install(TARGETS yampl
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/yampl
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/yampl
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/static/yampl
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/yampl
)

# Include plugins
execute_process(COMMAND sh "-c" "rm -R ../plugins; mkdir ../plugins")
if (WITH_SHM_PLUGIN STREQUAL ON)
    AddExtProjectGit("https://github.com/ntauth/yampl-shm" "../plugins")        
endif()

if (WITH_ZMQ_PLUGIN STREQUAL ON)
    AddExtProjectGit("https://github.com/ntauth/yampl-zmq" "../plugins")        
endif()

file(GLOB YAMPL_PLUGIN_CMAKE_FILES plugins/**/*.cmake)
foreach(YAMPL_CMAKE_FILE ${YAMPL_PLUGIN_CMAKE_FILES})
    include(${YAMPL_CMAKE_FILE})
endforeach(YAMPL_CMAKE_FILE)


# Copy the uninstall script over to the build directory
execute_process(COMMAND cp ../cmake/uninstall.sh ./)
execute_process(COMMAND chmod +x ./uninstall.sh)
