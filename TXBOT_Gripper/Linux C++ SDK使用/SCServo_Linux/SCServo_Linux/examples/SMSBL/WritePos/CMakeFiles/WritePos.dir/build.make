# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/hty/Applications/cmake-install/bin/cmake

# The command to remove a file.
RM = /home/hty/Applications/cmake-install/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos"

# Include any dependencies generated for this target.
include CMakeFiles/WritePos.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/WritePos.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/WritePos.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/WritePos.dir/flags.make

CMakeFiles/WritePos.dir/WritePos.cpp.o: CMakeFiles/WritePos.dir/flags.make
CMakeFiles/WritePos.dir/WritePos.cpp.o: WritePos.cpp
CMakeFiles/WritePos.dir/WritePos.cpp.o: CMakeFiles/WritePos.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/WritePos.dir/WritePos.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/WritePos.dir/WritePos.cpp.o -MF CMakeFiles/WritePos.dir/WritePos.cpp.o.d -o CMakeFiles/WritePos.dir/WritePos.cpp.o -c "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos/WritePos.cpp"

CMakeFiles/WritePos.dir/WritePos.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WritePos.dir/WritePos.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos/WritePos.cpp" > CMakeFiles/WritePos.dir/WritePos.cpp.i

CMakeFiles/WritePos.dir/WritePos.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WritePos.dir/WritePos.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos/WritePos.cpp" -o CMakeFiles/WritePos.dir/WritePos.cpp.s

# Object files for target WritePos
WritePos_OBJECTS = \
"CMakeFiles/WritePos.dir/WritePos.cpp.o"

# External object files for target WritePos
WritePos_EXTERNAL_OBJECTS =

WritePos: CMakeFiles/WritePos.dir/WritePos.cpp.o
WritePos: CMakeFiles/WritePos.dir/build.make
WritePos: CMakeFiles/WritePos.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable WritePos"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/WritePos.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/WritePos.dir/build: WritePos
.PHONY : CMakeFiles/WritePos.dir/build

CMakeFiles/WritePos.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/WritePos.dir/cmake_clean.cmake
.PHONY : CMakeFiles/WritePos.dir/clean

CMakeFiles/WritePos.dir/depend:
	cd "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos" "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos" "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos" "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos" "/home/hty/Desktop/SRTP/TXBOT_Gripper/Linux C++ SDK使用/SCServo_Linux/SCServo_Linux/examples/SMSBL/WritePos/CMakeFiles/WritePos.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/WritePos.dir/depend

