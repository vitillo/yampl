##
# @author: Ayoub Chouak (a.chouak@protonmail.com)
# @brief:  CMake utilities for YAMPL
##

include(ExternalProject)
include(CMakeParseArguments)

#
# @brief: Function to pull git external projects
#
# @param repo_url: Url to the repository
<<<<<<< HEAD
# @param repo_tag: Tag of the repository
# @param unpack_dst: Path of the directory where to unpack the binaries
#
# @usage: AddExtProjectGit("<url to git repository>", "<tag>", "<install destination>")
#
function(AddExtProjectGit repo_url repo_tag unpack_dst)

        message(STATUS "Pulling external git project from: ${repo_url}: ${repo_tag}")

        string(REGEX MATCH "([^\\/]+)[.]git$" proj_name ${repo_url})

=======
# @param unpack_dst: Path of the directory where to unpack the binaries
#
# @usage: AddExtProjectGit("<url to git repository>", "<clone destination>")
#
function(AddExtProjectGit repo_url clone_dst)
        message(STATUS "Pulling external git project from: ${repo_url}")
        execute_process(COMMAND sh "-c" "cd ${clone_dst} && git clone ${repo_url}")
>>>>>>> 9d580f85f336cdc90e250a61a2e22a7f386355e4
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
