# @Project YAMPL (Yet Another Message Passing Library)
#
# @Options:
# - WITH_EXAMPLES: Includes examples in the build process
# - WITH_TESTS: Includes tests in the build process
# - WITH_PLUGIN_<PluginId>: Include plugin yampl-<PluginId> in the build process
#
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

# Common source files for plugins
set(YAMPL_PLUGIN_COMMON_SRCS
        src/plugin/PluginArbiter.cpp
        src/utils/Globals.cpp
        src/utils/utils.cpp
        src/ISocket.cpp
)

# PThreads
find_package(Threads)

# Common libraries (for examples and tests)
set(YAMPL_COMMON_LIBS ${LIBRARY_OUTPUT_DIRECTORY}/libyampl.so dl uuid ${CMAKE_THREAD_LIBS_INIT})

# Include YAMPL dependencies
include(cmake/YamplDepends.cmake)

# Include directories
include_directories(include/ ${CMAKE_BINARY_DIR}/config)


add_library(yampl SHARED
        src/utils/utils.cpp
        src/utils/Globals.cpp
        src/plugin/PluginArbiter.cpp
        src/plugin/DynamicModule.cpp
        src/SocketFactory.cpp
        src/ISocket.cpp
)

option(WITH_EXAMPLES "Include examples" ON)

if(WITH_EXAMPLES)
    # Client-Server
    add_executable(client examples/client_server/client.cpp)
    add_executable(server examples/client_server/server.cpp)
    set_target_properties(client server
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/client_server"
    )
    target_link_libraries(client ${YAMPL_COMMON_LIBS})
    target_link_libraries(server ${YAMPL_COMMON_LIBS})
    add_dependencies(client yampl)
    add_dependencies(server yampl)
        
    # Benchmarks
    add_executable(benchmark examples/benchmarks/benchmark.cpp)
    set_target_properties(benchmark
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples/benchmark"
    )
    target_link_libraries(benchmark ${YAMPL_COMMON_LIBS})
    add_dependencies(benchmark yampl)

    # Add a custom command to run examples
    set(YAMPL_EXAMPLE_BENCHMARK $<TARGET_FILE:benchmark>)

    set(YAMPL_EXAMPLE_BENCHMARK0 "${YAMPL_EXAMPLE_BENCHMARK}")
    set(YAMPL_EXAMPLE_BENCHMARK1 "${YAMPL_EXAMPLE_BENCHMARK} -y")
    set(YAMPL_EXAMPLE_BENCHMARK2 "${YAMPL_EXAMPLE_BENCHMARK} -y -m 32 -n 128 -s 250000")
    set(YAMPL_EXAMPLE_BENCHMARK3 "${YAMPL_EXAMPLE_BENCHMARK} -i pipe")
    set(YAMPL_EXAMPLE_BENCHMARK4 "${YAMPL_EXAMPLE_BENCHMARK} -i pipe -y")
    set(YAMPL_EXAMPLE_BENCHMARK5 "${YAMPL_EXAMPLE_BENCHMARK} -i pipe -m 32 -n 128 -s 250000")
    set(YAMPL_EXAMPLE_BENCHMARK6 "${YAMPL_EXAMPLE_BENCHMARK} -i shm")
    set(YAMPL_EXAMPLE_BENCHMARK7 "${YAMPL_EXAMPLE_BENCHMARK} -i shm -y")
    set(YAMPL_EXAMPLE_BENCHMARK8 "${YAMPL_EXAMPLE_BENCHMARK} -i shm -m 32 -n 128 -s 250000")
    set(YAMPL_EXAMPLE_BENCHMARK9 "${YAMPL_EXAMPLE_BENCHMARK} -c THREAD")
    set(YAMPL_EXAMPLE_BENCHMARK10 "${YAMPL_EXAMPLE_BENCHMARK} -c DISTRIBUTED")

    add_custom_target(example
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK0}'\\; ${YAMPL_EXAMPLE_BENCHMARK0}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK1}'\\; ${YAMPL_EXAMPLE_BENCHMARK1}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK2}'\\; ${YAMPL_EXAMPLE_BENCHMARK2}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK3}'\\; ${YAMPL_EXAMPLE_BENCHMARK3}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK4}'\\; ${YAMPL_EXAMPLE_BENCHMARK4}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK5}'\\; ${YAMPL_EXAMPLE_BENCHMARK5}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK6}'\\; ${YAMPL_EXAMPLE_BENCHMARK6}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK7}'\\; ${YAMPL_EXAMPLE_BENCHMARK7}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK8}'\\; ${YAMPL_EXAMPLE_BENCHMARK8}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK9}'\\; ${YAMPL_EXAMPLE_BENCHMARK9}\\; echo"
        COMMAND sh -c "echo '${YAMPL_EXAMPLE_BENCHMARK10}'\\; ${YAMPL_EXAMPLE_BENCHMARK10}\\; echo"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    )
endif()

option(WITH_TESTS "Include tests" ON)

if (WITH_TESTS)
    enable_testing()
endif()

if(WITH_TESTS)
    # PluginArbiter Test
    add_executable(pluginarbiter tests/pluginarbiter.cpp)
    set_target_properties(pluginarbiter
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
    )
    target_link_libraries(pluginarbiter ${YAMPL_COMMON_LIBS})

    add_dependencies(pluginarbiter yampl)
    add_test(NAME pluginarbiter COMMAND pluginarbiter)

    # Dynmodule Test
    add_executable(dynmodule tests/dynmodule.cpp)
    set_target_properties(dynmodule
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
    )
    target_link_libraries(dynmodule ${YAMPL_COMMON_LIBS})

    add_dependencies(dynmodule yampl)
    add_test(NAME dynmodule COMMAND dynmodule WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

    # Calls Test
    add_executable(calls tests/calls.cpp)
    set_target_properties(calls
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
    )
    target_link_libraries(calls ${YAMPL_COMMON_LIBS})

    add_dependencies(calls yampl)
    add_test(NAME calls COMMAND calls)

    # Dest Test
    add_executable(dest tests/dest.cpp)
    set_target_properties(dest
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
    )
    target_link_libraries(dest ${YAMPL_COMMON_LIBS})
        
    add_dependencies(dest yampl)
    add_test(NAME dest COMMAND dest)

    # Size Test
    add_executable(size tests/size.cpp)
    set_target_properties(size
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
    )
    target_link_libraries(size ${YAMPL_COMMON_LIBS})

    add_dependencies(size yampl)
    add_test(NAME size COMMAND size)
endif()

# Libraries
target_link_libraries(yampl
    ${CMAKE_THREAD_LIBS_INIT}
    uuid
)

## @Section Install step
set_target_properties(yampl PROPERTIES RESOURCE "${YAMPL_RESOURCES}")

# Configure yampl-env.sh.in
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

# Install examples
install(DIRECTORY ${CMAKE_BINARY_DIR}/examples
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        PATTERN "*"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
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

# Include plugins
file(GLOB_RECURSE YAMPL_PLUGIN_CMAKE_FILES "plugins/*.conf.cmake")
foreach(YAMPL_CMAKE_FILE ${YAMPL_PLUGIN_CMAKE_FILES})
    string(REGEX REPLACE "(\\.conf\\.cmake)$" "" YAMPL_PLUGIN_NAME ${YAMPL_CMAKE_FILE})
    include(${YAMPL_CMAKE_FILE})
endforeach(YAMPL_CMAKE_FILE)

## @Section Global config file
configure_file(config/config.h.in ${CMAKE_BINARY_DIR}/config/config.h)
