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
endfunction()

# @brief: Macro to set -std=c++14 independent of the CMake binary version
macro(USE_CXX14)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 14)
  endif ()
endmacro(USE_CXX14)
