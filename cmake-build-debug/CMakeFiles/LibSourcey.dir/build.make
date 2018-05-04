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
CMAKE_COMMAND = /usr/share/clion-2018.1.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ntauthority/Desktop/CERN-HSF/yampl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug

# Utility rule file for LibSourcey.

# Include the progress variables for this target.
include CMakeFiles/LibSourcey.dir/progress.make

CMakeFiles/LibSourcey: CMakeFiles/LibSourcey-complete


CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-install
CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-mkdir
CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-download
CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-update
CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-patch
CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-configure
CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-build
CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-install
CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-test
CMakeFiles/LibSourcey-complete: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-AddPluga
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'LibSourcey'"
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E make_directory /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles/LibSourcey-complete
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-done

LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-install: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing install step for 'LibSourcey'"
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-build && $(MAKE) install
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-build && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-install

LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'LibSourcey'"
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E make_directory /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/libscy
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E make_directory /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-build
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E make_directory /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E make_directory /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/tmp
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E make_directory /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E make_directory /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-mkdir

LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-download: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-gitinfo.txt
LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-download: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'LibSourcey'"
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -P /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/tmp/LibSourcey-gitclone.cmake
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-download

LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-update: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'LibSourcey'"
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/libscy && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E echo_append
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/libscy && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-update

LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-patch: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'LibSourcey'"
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/libscy && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E echo_append
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/libscy && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-patch

LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-configure: LibSourcey-prefix/tmp/LibSourcey-cfgcmd.txt
LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-configure: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-update
LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-configure: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'LibSourcey'"
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-build && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_APPLICATIONS=OFF -DBUILD_TESTS=OFF -DBUILD_SAMPLES=OFF -DBUILD_MODULES=OFF -DBUILD_MODULE_base=ON -DBUILD_MODULE_uv=ON -DBUILD_MODULE_pluga=ON -DWITH_OPENCV=OFF -DWITH_WEBRTC=OFF -DWITH_FFMPEG=OFF "-GCodeBlocks - Unix Makefiles" /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/libscy
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-build && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-configure

LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-build: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'LibSourcey'"
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-build && $(MAKE)
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-build && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-build

LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-test: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "No test step for 'LibSourcey'"
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-build && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E echo_append
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-build && /usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-test

LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-AddPluga: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Performing AddPluga step for 'LibSourcey'"
	git clone https://github.com/sourcey/pluga.git pluga
	mv pluga libscy/src/
	/usr/share/clion-2018.1.2/bin/cmake/bin/cmake -E touch /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-AddPluga

LibSourcey: CMakeFiles/LibSourcey
LibSourcey: CMakeFiles/LibSourcey-complete
LibSourcey: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-install
LibSourcey: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-mkdir
LibSourcey: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-download
LibSourcey: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-update
LibSourcey: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-patch
LibSourcey: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-configure
LibSourcey: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-build
LibSourcey: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-test
LibSourcey: LibSourcey-prefix/src/LibSourcey-stamp/LibSourcey-AddPluga
LibSourcey: CMakeFiles/LibSourcey.dir/build.make

.PHONY : LibSourcey

# Rule to build all files generated by this target.
CMakeFiles/LibSourcey.dir/build: LibSourcey

.PHONY : CMakeFiles/LibSourcey.dir/build

CMakeFiles/LibSourcey.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/LibSourcey.dir/cmake_clean.cmake
.PHONY : CMakeFiles/LibSourcey.dir/clean

CMakeFiles/LibSourcey.dir/depend:
	cd /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ntauthority/Desktop/CERN-HSF/yampl /home/ntauthority/Desktop/CERN-HSF/yampl /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug /home/ntauthority/Desktop/CERN-HSF/yampl/cmake-build-debug/CMakeFiles/LibSourcey.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/LibSourcey.dir/depend
