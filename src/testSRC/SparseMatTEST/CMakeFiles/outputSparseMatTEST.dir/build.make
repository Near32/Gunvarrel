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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST"

# Include any dependencies generated for this target.
include CMakeFiles/outputSparseMatTEST.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/outputSparseMatTEST.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/outputSparseMatTEST.dir/flags.make

CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o: CMakeFiles/outputSparseMatTEST.dir/flags.make
CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o: mainSparseMatTEST.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o -c "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST/mainSparseMatTEST.cpp"

CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST/mainSparseMatTEST.cpp" > CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.i

CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST/mainSparseMatTEST.cpp" -o CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.s

CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o.requires:
.PHONY : CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o.requires

CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o.provides: CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o.requires
	$(MAKE) -f CMakeFiles/outputSparseMatTEST.dir/build.make CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o.provides.build
.PHONY : CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o.provides

CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o.provides.build: CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o

# Object files for target outputSparseMatTEST
outputSparseMatTEST_OBJECTS = \
"CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o"

# External object files for target outputSparseMatTEST
outputSparseMatTEST_EXTERNAL_OBJECTS =

bin/outputSparseMatTEST: CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o
bin/outputSparseMatTEST: CMakeFiles/outputSparseMatTEST.dir/build.make
bin/outputSparseMatTEST: CMakeFiles/outputSparseMatTEST.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bin/outputSparseMatTEST"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/outputSparseMatTEST.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/outputSparseMatTEST.dir/build: bin/outputSparseMatTEST
.PHONY : CMakeFiles/outputSparseMatTEST.dir/build

CMakeFiles/outputSparseMatTEST.dir/requires: CMakeFiles/outputSparseMatTEST.dir/mainSparseMatTEST.cpp.o.requires
.PHONY : CMakeFiles/outputSparseMatTEST.dir/requires

CMakeFiles/outputSparseMatTEST.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/outputSparseMatTEST.dir/cmake_clean.cmake
.PHONY : CMakeFiles/outputSparseMatTEST.dir/clean

CMakeFiles/outputSparseMatTEST.dir/depend:
	cd "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST" "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST" "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST" "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST" "/home/kevin/Documents/IS 3A/PLT/DEPOT/Gunvarrel/src/testSRC/SparseMatTEST/CMakeFiles/outputSparseMatTEST.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/outputSparseMatTEST.dir/depend

