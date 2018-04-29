# Install script for directory: /home/ntauthority/Desktop/CERN-HSF/yampl

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/yampl/libyampl.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/yampl/libyampl.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/yampl/libyampl.so"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/yampl/libyampl.so")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/yampl" TYPE SHARED_LIBRARY FILES "/home/ntauthority/Desktop/CERN-HSF/yampl/build/libyampl.so")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/yampl/libyampl.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/yampl/libyampl.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/usr/local/lib/yampl/libyampl.so"
         OLD_RPATH "/usr/local/lib:/usr/local/share/libsourcey/vendor/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/yampl/libyampl.so")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/include/yampl/Channel.h;/usr/local/include/yampl/Exceptions.h;/usr/local/include/yampl/ISocket.h;/usr/local/include/yampl/ISocketFactory.h;/usr/local/include/yampl/SocketFactory.h;/usr/local/include/yampl/ClientSocket.h;/usr/local/include/yampl/ServerSocketBase.h;/usr/local/include/yampl/ServiceSocketBase.h;/usr/local/include/yampl/SimpleClientSocket.h;/usr/local/include/yampl/SimpleServerSocket.h;/usr/local/include/yampl/ClientSocket.h;/usr/local/include/yampl/ConsumerSocket.h;/usr/local/include/yampl/PipeSocketBase.h;/usr/local/include/yampl/ProducerSocket.h;/usr/local/include/yampl/ServerSocket.h;/usr/local/include/yampl/SimpleClientSocket.h;/usr/local/include/yampl/SimpleServerSocket.h;/usr/local/include/yampl/SocketFactory.h;/usr/local/include/yampl/ClientSocket.h;/usr/local/include/yampl/ConsumerSocket.h;/usr/local/include/yampl/PipeSocketBase.h;/usr/local/include/yampl/ProducerSocket.h;/usr/local/include/yampl/ServerSocket.h;/usr/local/include/yampl/SimpleClientSocket.h;/usr/local/include/yampl/SimpleServerSocket.h;/usr/local/include/yampl/SocketFactory.h;/usr/local/include/yampl/Futex.h;/usr/local/include/yampl/Poller.h;/usr/local/include/yampl/RawPipe.h;/usr/local/include/yampl/RingBuffer.h;/usr/local/include/yampl/Semaphore.h;/usr/local/include/yampl/SharedMemory.h;/usr/local/include/yampl/SpinLock.h;/usr/local/include/yampl/Thread.h;/usr/local/include/yampl/UUID.h;/usr/local/include/yampl/utils.h;/usr/local/include/yampl/ClientSocket.h;/usr/local/include/yampl/ServerSocket.h;/usr/local/include/yampl/SocketBase.h;/usr/local/include/yampl/SocketFactory.h;/usr/local/include/yampl/IPlugin.hpp;/usr/local/include/yampl/PluginArbiter.hpp")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/include/yampl" TYPE FILE FILES
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/Channel.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/Exceptions.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/ISocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/ISocketFactory.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/SocketFactory.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/generic/ClientSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/generic/ServerSocketBase.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/generic/ServiceSocketBase.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/generic/SimpleClientSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/generic/SimpleServerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/pipe/ClientSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/pipe/ConsumerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/pipe/PipeSocketBase.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/pipe/ProducerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/pipe/ServerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/pipe/SimpleClientSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/pipe/SimpleServerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/pipe/SocketFactory.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/shm/ClientSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/shm/ConsumerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/shm/PipeSocketBase.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/shm/ProducerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/shm/ServerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/shm/SimpleClientSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/shm/SimpleServerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/shm/SocketFactory.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/Futex.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/Poller.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/RawPipe.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/RingBuffer.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/Semaphore.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/SharedMemory.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/SpinLock.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/Thread.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/UUID.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/utils/utils.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/zeromq/ClientSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/zeromq/ServerSocket.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/zeromq/SocketBase.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/zeromq/SocketFactory.h"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/plugin/IPlugin.hpp"
    "/home/ntauthority/Desktop/CERN-HSF/yampl/include/yampl/plugin/PluginArbiter.hpp"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/yampl/plugins/shm/libyampl-shm.yam" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/yampl/plugins/shm/libyampl-shm.yam")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/local/lib/yampl/plugins/shm/libyampl-shm.yam"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/usr/local/lib/yampl/plugins/shm/libyampl-shm.yam")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/usr/local/lib/yampl/plugins/shm" TYPE SHARED_LIBRARY FILES "/home/ntauthority/Desktop/CERN-HSF/yampl/build/plugins/shm/libyampl-shm.yam")
  if(EXISTS "$ENV{DESTDIR}/usr/local/lib/yampl/plugins/shm/libyampl-shm.yam" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/local/lib/yampl/plugins/shm/libyampl-shm.yam")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/local/lib/yampl/plugins/shm/libyampl-shm.yam")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/ntauthority/Desktop/CERN-HSF/yampl/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
