# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ntauthority/Desktop/CERN-HSF/yampl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ntauthority/Desktop/CERN-HSF/yampl/build

# Include any dependencies generated for this target.
include CMakeFiles/yampl-shm.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/yampl-shm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/yampl-shm.dir/flags.make

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o: CMakeFiles/yampl-shm.dir/flags.make
CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o: ../plugins/yampl-shm/src/ServerSocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o -c /home/ntauthority/Desktop/CERN-HSF/yampl/plugins/yampl-shm/src/ServerSocket.cpp

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ntauthority/Desktop/CERN-HSF/yampl/plugins/yampl-shm/src/ServerSocket.cpp > CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.i

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ntauthority/Desktop/CERN-HSF/yampl/plugins/yampl-shm/src/ServerSocket.cpp -o CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.s

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o.requires:

.PHONY : CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o.requires

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o.provides: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o.requires
	$(MAKE) -f CMakeFiles/yampl-shm.dir/build.make CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o.provides.build
.PHONY : CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o.provides

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o.provides.build: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o


CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o: CMakeFiles/yampl-shm.dir/flags.make
CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o: ../plugins/yampl-shm/src/SHMSocketBase.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o -c /home/ntauthority/Desktop/CERN-HSF/yampl/plugins/yampl-shm/src/SHMSocketBase.cpp

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ntauthority/Desktop/CERN-HSF/yampl/plugins/yampl-shm/src/SHMSocketBase.cpp > CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.i

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ntauthority/Desktop/CERN-HSF/yampl/plugins/yampl-shm/src/SHMSocketBase.cpp -o CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.s

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o.requires:

.PHONY : CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o.requires

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o.provides: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o.requires
	$(MAKE) -f CMakeFiles/yampl-shm.dir/build.make CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o.provides.build
.PHONY : CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o.provides

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o.provides.build: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o


CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o: CMakeFiles/yampl-shm.dir/flags.make
CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o: ../plugins/yampl-shm/src/SHMSocketFactory.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o -c /home/ntauthority/Desktop/CERN-HSF/yampl/plugins/yampl-shm/src/SHMSocketFactory.cpp

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ntauthority/Desktop/CERN-HSF/yampl/plugins/yampl-shm/src/SHMSocketFactory.cpp > CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.i

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ntauthority/Desktop/CERN-HSF/yampl/plugins/yampl-shm/src/SHMSocketFactory.cpp -o CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.s

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o.requires:

.PHONY : CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o.requires

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o.provides: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o.requires
	$(MAKE) -f CMakeFiles/yampl-shm.dir/build.make CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o.provides.build
.PHONY : CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o.provides

CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o.provides.build: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o


CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o: CMakeFiles/yampl-shm.dir/flags.make
CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o: ../src/ISocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o -c /home/ntauthority/Desktop/CERN-HSF/yampl/src/ISocket.cpp

CMakeFiles/yampl-shm.dir/src/ISocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yampl-shm.dir/src/ISocket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ntauthority/Desktop/CERN-HSF/yampl/src/ISocket.cpp > CMakeFiles/yampl-shm.dir/src/ISocket.cpp.i

CMakeFiles/yampl-shm.dir/src/ISocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yampl-shm.dir/src/ISocket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ntauthority/Desktop/CERN-HSF/yampl/src/ISocket.cpp -o CMakeFiles/yampl-shm.dir/src/ISocket.cpp.s

CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o.requires:

.PHONY : CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o.requires

CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o.provides: CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o.requires
	$(MAKE) -f CMakeFiles/yampl-shm.dir/build.make CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o.provides.build
.PHONY : CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o.provides

CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o.provides.build: CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o


CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o: CMakeFiles/yampl-shm.dir/flags.make
CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o: ../src/plugin/PluginArbiter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o -c /home/ntauthority/Desktop/CERN-HSF/yampl/src/plugin/PluginArbiter.cpp

CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ntauthority/Desktop/CERN-HSF/yampl/src/plugin/PluginArbiter.cpp > CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.i

CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ntauthority/Desktop/CERN-HSF/yampl/src/plugin/PluginArbiter.cpp -o CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.s

CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o.requires:

.PHONY : CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o.requires

CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o.provides: CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o.requires
	$(MAKE) -f CMakeFiles/yampl-shm.dir/build.make CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o.provides.build
.PHONY : CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o.provides

CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o.provides.build: CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o


# Object files for target yampl-shm
yampl__shm_OBJECTS = \
"CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o" \
"CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o" \
"CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o" \
"CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o" \
"CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o"

# External object files for target yampl-shm
yampl__shm_EXTERNAL_OBJECTS =

plugins/yampl-shm/libyampl-shm.yam: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o
plugins/yampl-shm/libyampl-shm.yam: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o
plugins/yampl-shm/libyampl-shm.yam: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o
plugins/yampl-shm/libyampl-shm.yam: CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o
plugins/yampl-shm/libyampl-shm.yam: CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o
plugins/yampl-shm/libyampl-shm.yam: CMakeFiles/yampl-shm.dir/build.make
plugins/yampl-shm/libyampl-shm.yam: CMakeFiles/yampl-shm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX shared library plugins/yampl-shm/libyampl-shm.yam"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/yampl-shm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/yampl-shm.dir/build: plugins/yampl-shm/libyampl-shm.yam

.PHONY : CMakeFiles/yampl-shm.dir/build

CMakeFiles/yampl-shm.dir/requires: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/ServerSocket.cpp.o.requires
CMakeFiles/yampl-shm.dir/requires: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketBase.cpp.o.requires
CMakeFiles/yampl-shm.dir/requires: CMakeFiles/yampl-shm.dir/plugins/yampl-shm/src/SHMSocketFactory.cpp.o.requires
CMakeFiles/yampl-shm.dir/requires: CMakeFiles/yampl-shm.dir/src/ISocket.cpp.o.requires
CMakeFiles/yampl-shm.dir/requires: CMakeFiles/yampl-shm.dir/src/plugin/PluginArbiter.cpp.o.requires

.PHONY : CMakeFiles/yampl-shm.dir/requires

CMakeFiles/yampl-shm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/yampl-shm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/yampl-shm.dir/clean

CMakeFiles/yampl-shm.dir/depend:
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ntauthority/Desktop/CERN-HSF/yampl /home/ntauthority/Desktop/CERN-HSF/yampl /home/ntauthority/Desktop/CERN-HSF/yampl/build /home/ntauthority/Desktop/CERN-HSF/yampl/build /home/ntauthority/Desktop/CERN-HSF/yampl/build/CMakeFiles/yampl-shm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/yampl-shm.dir/depend
