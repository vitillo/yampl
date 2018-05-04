# Install script for directory: /home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy/vendor/minizip

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RELEASE")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibsx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/share/libsourcey/vendor/lib/libminizip.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/share/libsourcey/vendor/lib/libminizip.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/share/libsourcey/vendor/lib/libminizip.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/share/libsourcey/vendor/lib/libminizip.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/share/libsourcey/vendor/lib" TYPE SHARED_LIBRARY FILES "/home/ntauthority/Desktop/CERN-HSF/yampl/build/LibSourcey-prefix/src/LibSourcey-build/vendor/minizip/libminizip.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/share/libsourcey/vendor/lib/libminizip.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/share/libsourcey/vendor/lib/libminizip.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/usr/local/share/libsourcey/vendor/lib/libminizip.so"
         OLD_RPATH "/home/ntauthority/Desktop/CERN-HSF/yampl/build/LibSourcey-prefix/src/LibSourcey-build/vendor/zlib:/usr/local/share/libsourcey/vendor/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/share/libsourcey/vendor/lib/libminizip.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/share/libsourcey/vendor/include/crypt.h;/usr/local/share/libsourcey/vendor/include/ioapi.h;/usr/local/share/libsourcey/vendor/include/mztools.h;/usr/local/share/libsourcey/vendor/include/unzip.h;/usr/local/share/libsourcey/vendor/include/zip.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/share/libsourcey/vendor/include" TYPE FILE FILES
    "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy/vendor/minizip/crypt.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy/vendor/minizip/ioapi.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy/vendor/minizip/mztools.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy/vendor/minizip/unzip.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libscy/vendor/minizip/zip.h"
    )
endif()

