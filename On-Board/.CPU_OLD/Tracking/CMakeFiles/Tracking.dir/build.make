# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/pi/Code/Tracking

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Code/Tracking

# Include any dependencies generated for this target.
include CMakeFiles/Tracking.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Tracking.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Tracking.dir/flags.make

CMakeFiles/Tracking.dir/Tracking.cpp.o: CMakeFiles/Tracking.dir/flags.make
CMakeFiles/Tracking.dir/Tracking.cpp.o: Tracking.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/Code/Tracking/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Tracking.dir/Tracking.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Tracking.dir/Tracking.cpp.o -c /home/pi/Code/Tracking/Tracking.cpp

CMakeFiles/Tracking.dir/Tracking.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tracking.dir/Tracking.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pi/Code/Tracking/Tracking.cpp > CMakeFiles/Tracking.dir/Tracking.cpp.i

CMakeFiles/Tracking.dir/Tracking.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tracking.dir/Tracking.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pi/Code/Tracking/Tracking.cpp -o CMakeFiles/Tracking.dir/Tracking.cpp.s

CMakeFiles/Tracking.dir/Tracking.cpp.o.requires:
.PHONY : CMakeFiles/Tracking.dir/Tracking.cpp.o.requires

CMakeFiles/Tracking.dir/Tracking.cpp.o.provides: CMakeFiles/Tracking.dir/Tracking.cpp.o.requires
	$(MAKE) -f CMakeFiles/Tracking.dir/build.make CMakeFiles/Tracking.dir/Tracking.cpp.o.provides.build
.PHONY : CMakeFiles/Tracking.dir/Tracking.cpp.o.provides

CMakeFiles/Tracking.dir/Tracking.cpp.o.provides.build: CMakeFiles/Tracking.dir/Tracking.cpp.o

# Object files for target Tracking
Tracking_OBJECTS = \
"CMakeFiles/Tracking.dir/Tracking.cpp.o"

# External object files for target Tracking
Tracking_EXTERNAL_OBJECTS =

Tracking: CMakeFiles/Tracking.dir/Tracking.cpp.o
Tracking: CMakeFiles/Tracking.dir/build.make
Tracking: /usr/local/lib/libopencv_videostab.so.2.4.7
Tracking: /usr/local/lib/libopencv_video.so.2.4.7
Tracking: /usr/local/lib/libopencv_ts.a
Tracking: /usr/local/lib/libopencv_superres.so.2.4.7
Tracking: /usr/local/lib/libopencv_stitching.so.2.4.7
Tracking: /usr/local/lib/libopencv_photo.so.2.4.7
Tracking: /usr/local/lib/libopencv_ocl.so.2.4.7
Tracking: /usr/local/lib/libopencv_objdetect.so.2.4.7
Tracking: /usr/local/lib/libopencv_nonfree.so.2.4.7
Tracking: /usr/local/lib/libopencv_ml.so.2.4.7
Tracking: /usr/local/lib/libopencv_legacy.so.2.4.7
Tracking: /usr/local/lib/libopencv_imgproc.so.2.4.7
Tracking: /usr/local/lib/libopencv_highgui.so.2.4.7
Tracking: /usr/local/lib/libopencv_gpu.so.2.4.7
Tracking: /usr/local/lib/libopencv_flann.so.2.4.7
Tracking: /usr/local/lib/libopencv_features2d.so.2.4.7
Tracking: /usr/local/lib/libopencv_core.so.2.4.7
Tracking: /usr/local/lib/libopencv_contrib.so.2.4.7
Tracking: /usr/local/lib/libopencv_calib3d.so.2.4.7
Tracking: /usr/local/lib/libopencv_nonfree.so.2.4.7
Tracking: /usr/local/lib/libopencv_ocl.so.2.4.7
Tracking: /usr/local/lib/libopencv_gpu.so.2.4.7
Tracking: /usr/local/lib/libopencv_photo.so.2.4.7
Tracking: /usr/local/lib/libopencv_objdetect.so.2.4.7
Tracking: /usr/local/lib/libopencv_legacy.so.2.4.7
Tracking: /usr/local/lib/libopencv_video.so.2.4.7
Tracking: /usr/local/lib/libopencv_ml.so.2.4.7
Tracking: /usr/local/lib/libopencv_calib3d.so.2.4.7
Tracking: /usr/local/lib/libopencv_features2d.so.2.4.7
Tracking: /usr/local/lib/libopencv_highgui.so.2.4.7
Tracking: /usr/local/lib/libopencv_imgproc.so.2.4.7
Tracking: /usr/local/lib/libopencv_flann.so.2.4.7
Tracking: /usr/local/lib/libopencv_core.so.2.4.7
Tracking: CMakeFiles/Tracking.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Tracking"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Tracking.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Tracking.dir/build: Tracking
.PHONY : CMakeFiles/Tracking.dir/build

CMakeFiles/Tracking.dir/requires: CMakeFiles/Tracking.dir/Tracking.cpp.o.requires
.PHONY : CMakeFiles/Tracking.dir/requires

CMakeFiles/Tracking.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Tracking.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Tracking.dir/clean

CMakeFiles/Tracking.dir/depend:
	cd /home/pi/Code/Tracking && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Code/Tracking /home/pi/Code/Tracking /home/pi/Code/Tracking /home/pi/Code/Tracking /home/pi/Code/Tracking/CMakeFiles/Tracking.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Tracking.dir/depend
