message(STATUS "
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
")
message(STATUS "Configuring...")

# Preamble
cmake_minimum_required(VERSION 2.8.10)
project(yampl)

# Use C++14
include(YamplMacros)
USE_CXX14()

# Common source files for plugins
set(YAMPL_PLUGIN_COMMON_SRCS src/ISocket.cpp src/plugin/PluginArbiter.cpp)

# Common libraries (for examples and tests)
set(YAMPL_COMMON_LIBS ${CMAKE_BINARY_DIR}/libyampl.so uuid pthread)

# Pthreads
find_package(Threads)

# Include YAMPL dependencies
include(YamplDepends)

# Include libsourcey.pc
# execute_process(COMMAND pkg-config --libs --cflags libsourcey OUTPUT_VARIABLE LIBSCY_PKG)
# message(STATUS ${LIBSCY_PKG})
# set(CMAKE_CXX_FLAGS ${LIBSCY_PKG})

# Include directories
include_directories(include/ zeromq/include
        /usr/local/include
        /usr/local/share/libsourcey/vendor/include
)

# Library directiories
link_directories(/usr/local/lib
        /usr/local/share/libsourcey/vendor/lib
)

# Source files
add_library(yampl SHARED
        include/yampl/generic/ClientSocket.h
        include/yampl/generic/ServerSocketBase.h
        include/yampl/generic/ServiceSocketBase.h
        include/yampl/generic/SimpleClientSocket.h
        include/yampl/generic/SimpleServerSocket.h
        include/yampl/pipe/ClientSocket.h
        include/yampl/pipe/ConsumerSocket.h
        include/yampl/pipe/PipeSocketBase.h
        include/yampl/pipe/ProducerSocket.h
        include/yampl/pipe/ServerSocket.h
        include/yampl/pipe/SimpleClientSocket.h
        include/yampl/pipe/SimpleServerSocket.h
        include/yampl/pipe/SocketFactory.h
        include/yampl/utils/Futex.h
        include/yampl/utils/Poller.h
        include/yampl/utils/RawPipe.h
        include/yampl/utils/RingBuffer.h
        include/yampl/utils/Semaphore.h
        include/yampl/utils/SharedMemory.h
        include/yampl/utils/SpinLock.h
        include/yampl/utils/Thread.h
        include/yampl/utils/utils.h
        include/yampl/utils/UUID.h
        include/yampl/Channel.h
        include/yampl/Exceptions.h
        include/yampl/ISocket.h
        include/yampl/ISocketFactory.h
        include/yampl/SocketFactory.h
        include/yampl.h
        src/pipe/PipeSocketBase.cpp
        src/pipe/PipeSocketFactory.cpp
        src/plugin/PluginArbiter.cpp
        src/SocketFactory.cpp
        src/ISocket.cpp
        zeromq/builds/msvc/platform.hpp
        zeromq/include/zmq.h
        zeromq/include/zmq.hpp
        zeromq/include/zmq_utils.h
        zeromq/src/address.cpp
        zeromq/src/address.hpp
        zeromq/src/array.hpp
        zeromq/src/atomic_counter.hpp
        zeromq/src/atomic_ptr.hpp
        zeromq/src/blob.hpp
        zeromq/src/clock.cpp
        zeromq/src/clock.hpp
        zeromq/src/command.hpp
        zeromq/src/config.hpp
        zeromq/src/ctx.cpp
        zeromq/src/ctx.hpp
        zeromq/src/dealer.cpp
        zeromq/src/dealer.hpp
        zeromq/src/decoder.cpp
        zeromq/src/decoder.hpp
        zeromq/src/devpoll.cpp
        zeromq/src/devpoll.hpp
        zeromq/src/dist.cpp
        zeromq/src/dist.hpp
        zeromq/src/encoder.cpp
        zeromq/src/encoder.hpp
        zeromq/src/epoll.cpp
        zeromq/src/epoll.hpp
        zeromq/src/err.cpp
        zeromq/src/err.hpp
        zeromq/src/fd.hpp
        zeromq/src/fq.cpp
        zeromq/src/fq.hpp
        zeromq/src/i_decoder.hpp
        zeromq/src/i_encoder.hpp
        zeromq/src/i_engine.hpp
        zeromq/src/i_msg_sink.hpp
        zeromq/src/i_msg_source.hpp
        zeromq/src/i_poll_events.hpp
        zeromq/src/io_object.cpp
        zeromq/src/io_object.hpp
        zeromq/src/io_thread.cpp
        zeromq/src/io_thread.hpp
        zeromq/src/ip.cpp
        zeromq/src/ip.hpp
        zeromq/src/ipc_address.cpp
        zeromq/src/ipc_address.hpp
        zeromq/src/ipc_connecter.cpp
        zeromq/src/ipc_connecter.hpp
        zeromq/src/ipc_listener.cpp
        zeromq/src/ipc_listener.hpp
        zeromq/src/kqueue.cpp
        zeromq/src/kqueue.hpp
        zeromq/src/lb.cpp
        zeromq/src/lb.hpp
        zeromq/src/likely.hpp
        zeromq/src/mailbox.cpp
        zeromq/src/mailbox.hpp
        zeromq/src/msg.cpp
        zeromq/src/msg.hpp
        zeromq/src/mtrie.cpp
        zeromq/src/mtrie.hpp
        zeromq/src/mutex.hpp
        zeromq/src/object.cpp
        zeromq/src/object.hpp
        zeromq/src/options.cpp
        zeromq/src/options.hpp
        zeromq/src/own.cpp
        zeromq/src/own.hpp
        zeromq/src/pair.cpp
        zeromq/src/pair.hpp
        zeromq/src/pgm_receiver.cpp
        zeromq/src/pgm_receiver.hpp
        zeromq/src/pgm_sender.cpp
        zeromq/src/pgm_sender.hpp
        zeromq/src/pgm_socket.cpp
        zeromq/src/pgm_socket.hpp
        zeromq/src/pipe.cpp
        zeromq/src/pipe.hpp
        zeromq/src/poll.cpp
        zeromq/src/poll.hpp
        zeromq/src/poller.hpp
        zeromq/src/poller_base.cpp
        zeromq/src/poller_base.hpp
        zeromq/src/precompiled.cpp
        zeromq/src/precompiled.hpp
        zeromq/src/proxy.cpp
        zeromq/src/proxy.hpp
        zeromq/src/pub.cpp
        zeromq/src/pub.hpp
        zeromq/src/pull.cpp
        zeromq/src/pull.hpp
        zeromq/src/push.cpp
        zeromq/src/push.hpp
        zeromq/src/random.cpp
        zeromq/src/random.hpp
        zeromq/src/reaper.cpp
        zeromq/src/reaper.hpp
        zeromq/src/rep.cpp
        zeromq/src/rep.hpp
        zeromq/src/req.cpp
        zeromq/src/req.hpp
        zeromq/src/router.cpp
        zeromq/src/router.hpp
        zeromq/src/select.cpp
        zeromq/src/select.hpp
        zeromq/src/session_base.cpp
        zeromq/src/session_base.hpp
        zeromq/src/signaler.cpp
        zeromq/src/signaler.hpp
        zeromq/src/socket_base.cpp
        zeromq/src/socket_base.hpp
        zeromq/src/stdint.hpp
        zeromq/src/stream_engine.cpp
        zeromq/src/stream_engine.hpp
        zeromq/src/sub.cpp
        zeromq/src/sub.hpp
        zeromq/src/tcp.cpp
        zeromq/src/tcp.hpp
        zeromq/src/tcp_address.cpp
        zeromq/src/tcp_address.hpp
        zeromq/src/tcp_connecter.cpp
        zeromq/src/tcp_connecter.hpp
        zeromq/src/tcp_listener.cpp
        zeromq/src/tcp_listener.hpp
        zeromq/src/thread.cpp
        zeromq/src/thread.hpp
        zeromq/src/trie.cpp
        zeromq/src/trie.hpp
        zeromq/src/v1_decoder.cpp
        zeromq/src/v1_decoder.hpp
        zeromq/src/v1_encoder.cpp
        zeromq/src/v1_encoder.hpp
        zeromq/src/v1_protocol.hpp
        zeromq/src/windows.hpp
        zeromq/src/wire.hpp
        zeromq/src/xpub.cpp
        zeromq/src/xpub.hpp
        zeromq/src/xsub.cpp
        zeromq/src/xsub.hpp
        zeromq/src/ypipe.hpp
        zeromq/src/yqueue.hpp
        zeromq/src/zmq.cpp
        zeromq/src/zmq_utils.cpp
        zeromq/tests/testutil.hpp
)

if(WITH_EXAMPLES STREQUAL ON)
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
file(GLOB YAMPL_PLUGIN_CMAKE_FILES plugins/**/*.cmake)
foreach(YAMPL_CMAKE_FILE ${YAMPL_PLUGIN_CMAKE_FILES})
    include(${YAMPL_CMAKE_FILE})
endforeach(YAMPL_CMAKE_FILE)


# Copy the uninstall script over to the build directory
execute_process(COMMAND cp ../uninstall.sh ./)
execute_process(COMMAND chmod +x ./uninstall.sh)
