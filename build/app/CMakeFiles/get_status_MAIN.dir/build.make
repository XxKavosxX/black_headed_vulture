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
CMAKE_SOURCE_DIR = /home/maike/Github/black_headed_vulture

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maike/Github/black_headed_vulture/build

# Utility rule file for get_status_MAIN.

# Include the progress variables for this target.
include app/CMakeFiles/get_status_MAIN.dir/progress.make

app/CMakeFiles/get_status_MAIN:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/maike/Github/black_headed_vulture/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Get status from atmega328p"
	cd /home/maike/Github/black_headed_vulture/build/app && avrdude -p atmega328p -c arduino -P /dev/ttyACM0 -n -v

get_status_MAIN: app/CMakeFiles/get_status_MAIN
get_status_MAIN: app/CMakeFiles/get_status_MAIN.dir/build.make

.PHONY : get_status_MAIN

# Rule to build all files generated by this target.
app/CMakeFiles/get_status_MAIN.dir/build: get_status_MAIN

.PHONY : app/CMakeFiles/get_status_MAIN.dir/build

app/CMakeFiles/get_status_MAIN.dir/clean:
	cd /home/maike/Github/black_headed_vulture/build/app && $(CMAKE_COMMAND) -P CMakeFiles/get_status_MAIN.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/get_status_MAIN.dir/clean

app/CMakeFiles/get_status_MAIN.dir/depend:
	cd /home/maike/Github/black_headed_vulture/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maike/Github/black_headed_vulture /home/maike/Github/black_headed_vulture/app /home/maike/Github/black_headed_vulture/build /home/maike/Github/black_headed_vulture/build/app /home/maike/Github/black_headed_vulture/build/app/CMakeFiles/get_status_MAIN.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/CMakeFiles/get_status_MAIN.dir/depend

