# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.3.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.3.1\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Pat\CLionProjects\z80

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Pat\CLionProjects\z80\cmake-build-debug

# Include any dependencies generated for this target.
include zip/test/CMakeFiles/test.out.dir/depend.make

# Include the progress variables for this target.
include zip/test/CMakeFiles/test.out.dir/progress.make

# Include the compile flags for this target's objects.
include zip/test/CMakeFiles/test.out.dir/flags.make

zip/test/CMakeFiles/test.out.dir/test.c.obj: zip/test/CMakeFiles/test.out.dir/flags.make
zip/test/CMakeFiles/test.out.dir/test.c.obj: zip/test/CMakeFiles/test.out.dir/includes_C.rsp
zip/test/CMakeFiles/test.out.dir/test.c.obj: ../zip/test/test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Pat\CLionProjects\z80\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object zip/test/CMakeFiles/test.out.dir/test.c.obj"
	cd /d C:\Users\Pat\CLionProjects\z80\cmake-build-debug\zip\test && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\test.out.dir\test.c.obj   -c C:\Users\Pat\CLionProjects\z80\zip\test\test.c

zip/test/CMakeFiles/test.out.dir/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test.out.dir/test.c.i"
	cd /d C:\Users\Pat\CLionProjects\z80\cmake-build-debug\zip\test && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Pat\CLionProjects\z80\zip\test\test.c > CMakeFiles\test.out.dir\test.c.i

zip/test/CMakeFiles/test.out.dir/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test.out.dir/test.c.s"
	cd /d C:\Users\Pat\CLionProjects\z80\cmake-build-debug\zip\test && C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Pat\CLionProjects\z80\zip\test\test.c -o CMakeFiles\test.out.dir\test.c.s

# Object files for target test.out
test_out_OBJECTS = \
"CMakeFiles/test.out.dir/test.c.obj"

# External object files for target test.out
test_out_EXTERNAL_OBJECTS =

zip/test/test.out.exe: zip/test/CMakeFiles/test.out.dir/test.c.obj
zip/test/test.out.exe: zip/test/CMakeFiles/test.out.dir/build.make
zip/test/test.out.exe: zip/libzip.a
zip/test/test.out.exe: zip/test/CMakeFiles/test.out.dir/linklibs.rsp
zip/test/test.out.exe: zip/test/CMakeFiles/test.out.dir/objects1.rsp
zip/test/test.out.exe: zip/test/CMakeFiles/test.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Pat\CLionProjects\z80\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test.out.exe"
	cd /d C:\Users\Pat\CLionProjects\z80\cmake-build-debug\zip\test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\test.out.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
zip/test/CMakeFiles/test.out.dir/build: zip/test/test.out.exe

.PHONY : zip/test/CMakeFiles/test.out.dir/build

zip/test/CMakeFiles/test.out.dir/clean:
	cd /d C:\Users\Pat\CLionProjects\z80\cmake-build-debug\zip\test && $(CMAKE_COMMAND) -P CMakeFiles\test.out.dir\cmake_clean.cmake
.PHONY : zip/test/CMakeFiles/test.out.dir/clean

zip/test/CMakeFiles/test.out.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Pat\CLionProjects\z80 C:\Users\Pat\CLionProjects\z80\zip\test C:\Users\Pat\CLionProjects\z80\cmake-build-debug C:\Users\Pat\CLionProjects\z80\cmake-build-debug\zip\test C:\Users\Pat\CLionProjects\z80\cmake-build-debug\zip\test\CMakeFiles\test.out.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : zip/test/CMakeFiles/test.out.dir/depend

