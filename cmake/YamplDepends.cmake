# ZeroMQ + CppZMQ
include(ExternalProject)

if (NOT DEFINED WITH_PLUGIN_ZMQ OR WITH_PLUGIN_ZMQ)	    
    # Pull ZeroMQ
    set(ZEROMQ_ROOT_DIR ${CMAKE_BINARY_DIR}/zeromq)
    set(ZEROMQ_LIB_DIR ${ZEROMQ_ROOT_DIR}/bin/lib)
    set(ZEROMQ_SLIB_DIR ${ZEROMQ_ROOT_DIR}/bin/slib)
    set(ZEROMQ_LIB64_DIR ${ZEROMQ_ROOT_DIR}/bin/lib64)
    set(ZEROMQ_INCLUDE_DIR ${ZEROMQ_ROOT_DIR}/bin/include)

    ExternalProject_Add(ZeroMQ
        GIT_REPOSITORY "https://github.com/zeromq/libzmq"
        PREFIX ${ZEROMQ_ROOT_DIR}
        GIT_TAG "d062edd8c142384792955796329baf1e5a3377cd"
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        INSTALL_DIR ${ZEROMQ_ROOT_DIR}/bin
        CMAKE_ARGS -DZMQ_BUILD_TESTS=OFF -DWITH_PERF_TOOL=OFF -DENABLE_CPACK=OFF -DCMAKE_INSTALL_PREFIX=${ZEROMQ_ROOT_DIR}/bin
        BUILD_COMMAND make
        TEST_COMMAND ""
    )
    
    ExternalProject_Add_Step(ZeroMQ FixLibDir
        COMMAND sh -c "mkdir ${ZEROMQ_SLIB_DIR} && mv $(find ${ZEROMQ_LIB_DIR}*/ -name \"libzmq.a\") ${ZEROMQ_SLIB_DIR}"
        DEPENDEES install
    )

    # Pull CppZMQ
    set(CPPZMQ_ROOT ${CMAKE_BINARY_DIR}/cppzmq)
    set(CPPZMQ_INCLUDE_DIR ${CPPZMQ_ROOT}/src/CppZMQ)

    ExternalProject_Add(CppZMQ
        GIT_REPOSITORY "https://github.com/zeromq/cppzmq"
        PREFIX ${CPPZMQ_ROOT}
        GIT_TAG "6aa3ab686e916cb0e62df7fa7d12e0b13ae9fae6"
        CONFIGURE_COMMAND ""
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        CMAKE_ARGS ""
        BUILD_COMMAND ""
        TEST_COMMAND ""
        INSTALL_COMMAND ""
    )

    add_dependencies(CppZMQ ZeroMQ)
endif()
