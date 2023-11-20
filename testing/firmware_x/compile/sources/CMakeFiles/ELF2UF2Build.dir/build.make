# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/paulr/Projects/neo6502-firmware/firmware

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paulr/Projects/neo6502-firmware/firmware/compile

# Utility rule file for ELF2UF2Build.

# Include any custom commands dependencies for this target.
include sources/CMakeFiles/ELF2UF2Build.dir/compiler_depend.make

# Include the progress variables for this target.
include sources/CMakeFiles/ELF2UF2Build.dir/progress.make

sources/CMakeFiles/ELF2UF2Build: sources/CMakeFiles/ELF2UF2Build-complete

sources/CMakeFiles/ELF2UF2Build-complete: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-install
sources/CMakeFiles/ELF2UF2Build-complete: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir
sources/CMakeFiles/ELF2UF2Build-complete: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download
sources/CMakeFiles/ELF2UF2Build-complete: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update
sources/CMakeFiles/ELF2UF2Build-complete: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch
sources/CMakeFiles/ELF2UF2Build-complete: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure
sources/CMakeFiles/ELF2UF2Build-complete: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-build
sources/CMakeFiles/ELF2UF2Build-complete: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/paulr/Projects/neo6502-firmware/firmware/compile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'ELF2UF2Build'"
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E make_directory /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/CMakeFiles
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E touch /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/CMakeFiles/ELF2UF2Build-complete
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E touch /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-done

sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-build: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/paulr/Projects/neo6502-firmware/firmware/compile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing build step for 'ELF2UF2Build'"
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/elf2uf2 && $(MAKE)

sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure: sources/elf2uf2/tmp/ELF2UF2Build-cfgcmd.txt
sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/paulr/Projects/neo6502-firmware/firmware/compile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Performing configure step for 'ELF2UF2Build'"
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/elf2uf2 && /usr/bin/cmake -DCMAKE_MAKE_PROGRAM:FILEPATH=/usr/bin/make "-GUnix Makefiles" -S /aux/pico/pico-sdk/tools/elf2uf2 -B /home/paulr/Projects/neo6502-firmware/firmware/compile/elf2uf2
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/elf2uf2 && /usr/bin/cmake -E touch /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure

sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-source_dirinfo.txt
sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/paulr/Projects/neo6502-firmware/firmware/compile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'ELF2UF2Build'"
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E echo_append
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E touch /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download

sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-install: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/paulr/Projects/neo6502-firmware/firmware/compile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'ELF2UF2Build'"
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/elf2uf2 && /usr/bin/cmake -E echo_append

sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/paulr/Projects/neo6502-firmware/firmware/compile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'ELF2UF2Build'"
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -Dcfgdir= -P /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/elf2uf2/tmp/ELF2UF2Build-mkdirs.cmake
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E touch /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir

sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch-info.txt
sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/paulr/Projects/neo6502-firmware/firmware/compile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'ELF2UF2Build'"
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E echo_append
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E touch /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch

sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update-info.txt
sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/paulr/Projects/neo6502-firmware/firmware/compile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No update step for 'ELF2UF2Build'"
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E echo_append
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && /usr/bin/cmake -E touch /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update

ELF2UF2Build: sources/CMakeFiles/ELF2UF2Build
ELF2UF2Build: sources/CMakeFiles/ELF2UF2Build-complete
ELF2UF2Build: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-build
ELF2UF2Build: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-configure
ELF2UF2Build: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-download
ELF2UF2Build: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-install
ELF2UF2Build: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-mkdir
ELF2UF2Build: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-patch
ELF2UF2Build: sources/elf2uf2/src/ELF2UF2Build-stamp/ELF2UF2Build-update
ELF2UF2Build: sources/CMakeFiles/ELF2UF2Build.dir/build.make
.PHONY : ELF2UF2Build

# Rule to build all files generated by this target.
sources/CMakeFiles/ELF2UF2Build.dir/build: ELF2UF2Build
.PHONY : sources/CMakeFiles/ELF2UF2Build.dir/build

sources/CMakeFiles/ELF2UF2Build.dir/clean:
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile/sources && $(CMAKE_COMMAND) -P CMakeFiles/ELF2UF2Build.dir/cmake_clean.cmake
.PHONY : sources/CMakeFiles/ELF2UF2Build.dir/clean

sources/CMakeFiles/ELF2UF2Build.dir/depend:
	cd /home/paulr/Projects/neo6502-firmware/firmware/compile && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paulr/Projects/neo6502-firmware/firmware /home/paulr/Projects/neo6502-firmware/firmware/sources /home/paulr/Projects/neo6502-firmware/firmware/compile /home/paulr/Projects/neo6502-firmware/firmware/compile/sources /home/paulr/Projects/neo6502-firmware/firmware/compile/sources/CMakeFiles/ELF2UF2Build.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : sources/CMakeFiles/ELF2UF2Build.dir/depend
