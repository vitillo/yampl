##
# @author: Ayoub Chouak (a.chouak@protonmail.com)
# @brief:  CMake utilities for YAMPL
##

include(ExternalProject)

#
# @brief: Function to pull git external projects 
# @usage: AddExtProjectGit("<url to git repository>", "<base directory>")
#
function(AddExtProjectGit repo_url prefix)
    execute_process(COMMAND sh "-c" "cd ${prefix} && git clone ${repo_url}")
    #ExternalProject_Add(${target}
    #    GIT_REPOSITORY ${repo_url}
    #    PREFIX ${proj_root}
    #    UPDATE_COMMAND ""
    #    PATCH_COMMAND ""
    #    BUILD_COMMAND "make"
    #    TEST_COMMAND ""
    #    CMAKE_ARGS ${cmake_args}
    #    CONFIGURE_COMMAND ""
    #    INSTALL_COMMAND "make install"
    # )
endfunction()

#
# @brief: Macro to set -std=c++14 independent of the CMake binary version
#
macro(USE_CXX14)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
  endif ()
endmacro(USE_CXX14)
