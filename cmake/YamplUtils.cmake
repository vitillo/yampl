##
# @author: Ayoub Chouak (a.chouak@protonmail.com)
# @brief:  CMake utilities for YAMPL
##

include(ExternalProject)

#
# @brief: Function to pull git external projects 
# @usage: AddExtProjectGit("<target name>", "<url to git repository>", "<base directory>")
#
function(AddExtProjectGit target repo_url prefix)
    string(TOLOWER ${target} target)
    set(proj_root ${CMAKE_BINARY_DIR}/${prefix}/${target})
     
    ExternalProject_Add(${target}
        GIT_REPOSITORY ${repo_url}
        PREFIX ${proj_root}
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        BUILD_COMMAND ""
        TEST_COMMAND ""
    )
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
