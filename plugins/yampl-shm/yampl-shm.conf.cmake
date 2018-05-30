# YAMPL-SHM
if (WITH_SHM_PLUGIN)
    add_library(yampl-shm SHARED
            ${CMAKE_CURRENT_LIST_DIR}/src/ServerSocket.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/SHMSocketBase.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/SHMSocketFactory.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/PluginMain.cpp
            ${YAMPL_PLUGIN_COMMON_SRCS}
    )

    target_include_directories(yampl-shm PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

    set_target_properties(yampl-shm
            PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins/yampl-shm"
    )

    install(TARGETS yampl-shm
            LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/plugins/yampl-shm
    )
endif()
