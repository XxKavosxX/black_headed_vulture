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

# Utility rule file for firmwaresize_MAIN.awk.

# Include the progress variables for this target.
include app/CMakeFiles/firmwaresize_MAIN.awk.dir/progress.make

app/CMakeFiles/firmwaresize_MAIN.awk:
	cd /home/maike/Github/black_headed_vulture/build/app && echo "BEGIN {ORS=\"\";print \"\\\\n\\\\033[1;33mFirmware size (\"}" > firmwaresize_MAIN.awk
	cd /home/maike/Github/black_headed_vulture/build/app && echo "/^Device/ {print \$$2 \") is...  \"}" >> firmwaresize_MAIN.awk
	cd /home/maike/Github/black_headed_vulture/build/app && echo "/^Program/ {print \"Flash (program): \" \$$2 \" \" \$$3 \" \" \$$4 \")  \"}" >> firmwaresize_MAIN.awk
	cd /home/maike/Github/black_headed_vulture/build/app && echo "/^Data/ {print \"RAM (globals): \" \$$2 \" \" \$$3 \" \" \$$4 \")  \"}" >> firmwaresize_MAIN.awk
	cd /home/maike/Github/black_headed_vulture/build/app && echo "END {print \"\\\\033[0m\\\\n\\\\n\"}" >> firmwaresize_MAIN.awk

firmwaresize_MAIN.awk: app/CMakeFiles/firmwaresize_MAIN.awk
firmwaresize_MAIN.awk: app/CMakeFiles/firmwaresize_MAIN.awk.dir/build.make

.PHONY : firmwaresize_MAIN.awk

# Rule to build all files generated by this target.
app/CMakeFiles/firmwaresize_MAIN.awk.dir/build: firmwaresize_MAIN.awk

.PHONY : app/CMakeFiles/firmwaresize_MAIN.awk.dir/build

app/CMakeFiles/firmwaresize_MAIN.awk.dir/clean:
	cd /home/maike/Github/black_headed_vulture/build/app && $(CMAKE_COMMAND) -P CMakeFiles/firmwaresize_MAIN.awk.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/firmwaresize_MAIN.awk.dir/clean

app/CMakeFiles/firmwaresize_MAIN.awk.dir/depend:
	cd /home/maike/Github/black_headed_vulture/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maike/Github/black_headed_vulture /home/maike/Github/black_headed_vulture/app /home/maike/Github/black_headed_vulture/build /home/maike/Github/black_headed_vulture/build/app /home/maike/Github/black_headed_vulture/build/app/CMakeFiles/firmwaresize_MAIN.awk.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : app/CMakeFiles/firmwaresize_MAIN.awk.dir/depend

