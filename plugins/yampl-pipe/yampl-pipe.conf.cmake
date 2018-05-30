# YAMPL-PIPE
if (WITH_PIPE_PLUGIN)
    add_library(yampl-pipe SHARED
            ${CMAKE_CURRENT_LIST_DIR}/src/PipeSocketBase.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/PipeSocketFactory.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/PluginMain.cpp
            ${YAMPL_PLUGIN_COMMON_SRCS}
    )

    target_include_directories(yampl-pipe PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

    set_target_properties(yampl-pipe
            PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/yampl-shm"
    )
    target_compile_options(yampl-pipe PRIVATE "-Wno-terminate")

    install(TARGETS yampl-pipe
            LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plugins/yampl-pipe
    )
endif()
