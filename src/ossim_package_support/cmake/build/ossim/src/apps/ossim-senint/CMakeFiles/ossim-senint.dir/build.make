# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vipul/ossim-svn/src/ossim_package_support/cmake

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vipul/ossim-svn/src/ossim_package_support/cmake/build

# Include any dependencies generated for this target.
include ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/depend.make

# Include the progress variables for this target.
include ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/progress.make

# Include the compile flags for this target's objects.
include ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/flags.make

ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o: ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/flags.make
ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o: /home/vipul/ossim-svn/src/ossim/src/apps/ossim-senint/ossim-senint.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vipul/ossim-svn/src/ossim_package_support/cmake/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o"
	cd /home/vipul/ossim-svn/src/ossim_package_support/cmake/build/ossim/src/apps/ossim-senint && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o -c /home/vipul/ossim-svn/src/ossim/src/apps/ossim-senint/ossim-senint.cpp

ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ossim-senint.dir/ossim-senint.cpp.i"
	cd /home/vipul/ossim-svn/src/ossim_package_support/cmake/build/ossim/src/apps/ossim-senint && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vipul/ossim-svn/src/ossim/src/apps/ossim-senint/ossim-senint.cpp > CMakeFiles/ossim-senint.dir/ossim-senint.cpp.i

ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ossim-senint.dir/ossim-senint.cpp.s"
	cd /home/vipul/ossim-svn/src/ossim_package_support/cmake/build/ossim/src/apps/ossim-senint && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vipul/ossim-svn/src/ossim/src/apps/ossim-senint/ossim-senint.cpp -o CMakeFiles/ossim-senint.dir/ossim-senint.cpp.s

ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o.requires:
.PHONY : ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o.requires

ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o.provides: ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o.requires
	$(MAKE) -f ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/build.make ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o.provides.build
.PHONY : ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o.provides

ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o.provides.build: ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o

# Object files for target ossim-senint
ossim__senint_OBJECTS = \
"CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o"

# External object files for target ossim-senint
ossim__senint_EXTERNAL_OBJECTS =

ossim-senint: ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o
ossim-senint: libossim.so.1.8.12
ossim-senint: /usr/lib/libOpenThreads.so
ossim-senint: /usr/lib/libjpeg.so
ossim-senint: /usr/lib/libtiff.so
ossim-senint: /usr/lib/libgeotiff.so
ossim-senint: /usr/lib/libOpenThreads.so
ossim-senint: /usr/lib/libfreetype.so
ossim-senint: /usr/lib/openmpi/lib/libmpi_cxx.so
ossim-senint: /usr/lib/openmpi/lib/libmpi.so
ossim-senint: /usr/lib/openmpi/lib/libopen-rte.so
ossim-senint: /usr/lib/openmpi/lib/libopen-pal.so
ossim-senint: /usr/lib/libdl.so
ossim-senint: /usr/lib/libnsl.so
ossim-senint: /usr/lib/libutil.so
ossim-senint: /usr/lib/libm.so
ossim-senint: /usr/lib/libz.so
ossim-senint: /usr/lib/libdl.so
ossim-senint: /usr/lib/libnsl.so
ossim-senint: /usr/lib/libutil.so
ossim-senint: /usr/lib/libm.so
ossim-senint: /usr/lib/libz.so
ossim-senint: ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/build.make
ossim-senint: ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../../../ossim-senint"
	cd /home/vipul/ossim-svn/src/ossim_package_support/cmake/build/ossim/src/apps/ossim-senint && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ossim-senint.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/build: ossim-senint
.PHONY : ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/build

ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/requires: ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/ossim-senint.cpp.o.requires
.PHONY : ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/requires

ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/clean:
	cd /home/vipul/ossim-svn/src/ossim_package_support/cmake/build/ossim/src/apps/ossim-senint && $(CMAKE_COMMAND) -P CMakeFiles/ossim-senint.dir/cmake_clean.cmake
.PHONY : ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/clean

ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/depend:
	cd /home/vipul/ossim-svn/src/ossim_package_support/cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vipul/ossim-svn/src/ossim_package_support/cmake /home/vipul/ossim-svn/src/ossim/src/apps/ossim-senint /home/vipul/ossim-svn/src/ossim_package_support/cmake/build /home/vipul/ossim-svn/src/ossim_package_support/cmake/build/ossim/src/apps/ossim-senint /home/vipul/ossim-svn/src/ossim_package_support/cmake/build/ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : ossim/src/apps/ossim-senint/CMakeFiles/ossim-senint.dir/depend

