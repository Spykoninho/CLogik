# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2024.2.2\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2024.2.2\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\mathf\Desktop\Cours\S1\C\CLogik

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CLogik.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CLogik.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CLogik.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CLogik.dir/flags.make

CMakeFiles/CLogik.dir/src/main.c.obj: CMakeFiles/CLogik.dir/flags.make
CMakeFiles/CLogik.dir/src/main.c.obj: C:/Users/mathf/Desktop/Cours/S1/C/CLogik/src/main.c
CMakeFiles/CLogik.dir/src/main.c.obj: CMakeFiles/CLogik.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/CLogik.dir/src/main.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/CLogik.dir/src/main.c.obj -MF CMakeFiles\CLogik.dir\src\main.c.obj.d -o CMakeFiles\CLogik.dir\src\main.c.obj -c C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\main.c

CMakeFiles/CLogik.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/CLogik.dir/src/main.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\main.c > CMakeFiles\CLogik.dir\src\main.c.i

CMakeFiles/CLogik.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/CLogik.dir/src/main.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\main.c -o CMakeFiles\CLogik.dir\src\main.c.s

CMakeFiles/CLogik.dir/src/lexer.c.obj: CMakeFiles/CLogik.dir/flags.make
CMakeFiles/CLogik.dir/src/lexer.c.obj: C:/Users/mathf/Desktop/Cours/S1/C/CLogik/src/lexer.c
CMakeFiles/CLogik.dir/src/lexer.c.obj: CMakeFiles/CLogik.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/CLogik.dir/src/lexer.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/CLogik.dir/src/lexer.c.obj -MF CMakeFiles\CLogik.dir\src\lexer.c.obj.d -o CMakeFiles\CLogik.dir\src\lexer.c.obj -c C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\lexer.c

CMakeFiles/CLogik.dir/src/lexer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/CLogik.dir/src/lexer.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\lexer.c > CMakeFiles\CLogik.dir\src\lexer.c.i

CMakeFiles/CLogik.dir/src/lexer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/CLogik.dir/src/lexer.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\lexer.c -o CMakeFiles\CLogik.dir\src\lexer.c.s

CMakeFiles/CLogik.dir/src/token.c.obj: CMakeFiles/CLogik.dir/flags.make
CMakeFiles/CLogik.dir/src/token.c.obj: C:/Users/mathf/Desktop/Cours/S1/C/CLogik/src/token.c
CMakeFiles/CLogik.dir/src/token.c.obj: CMakeFiles/CLogik.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/CLogik.dir/src/token.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/CLogik.dir/src/token.c.obj -MF CMakeFiles\CLogik.dir\src\token.c.obj.d -o CMakeFiles\CLogik.dir\src\token.c.obj -c C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\token.c

CMakeFiles/CLogik.dir/src/token.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/CLogik.dir/src/token.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\token.c > CMakeFiles\CLogik.dir\src\token.c.i

CMakeFiles/CLogik.dir/src/token.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/CLogik.dir/src/token.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\token.c -o CMakeFiles\CLogik.dir\src\token.c.s

CMakeFiles/CLogik.dir/src/parser.c.obj: CMakeFiles/CLogik.dir/flags.make
CMakeFiles/CLogik.dir/src/parser.c.obj: C:/Users/mathf/Desktop/Cours/S1/C/CLogik/src/parser.c
CMakeFiles/CLogik.dir/src/parser.c.obj: CMakeFiles/CLogik.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/CLogik.dir/src/parser.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/CLogik.dir/src/parser.c.obj -MF CMakeFiles\CLogik.dir\src\parser.c.obj.d -o CMakeFiles\CLogik.dir\src\parser.c.obj -c C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\parser.c

CMakeFiles/CLogik.dir/src/parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/CLogik.dir/src/parser.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\parser.c > CMakeFiles\CLogik.dir\src\parser.c.i

CMakeFiles/CLogik.dir/src/parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/CLogik.dir/src/parser.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\parser.c -o CMakeFiles\CLogik.dir\src\parser.c.s

CMakeFiles/CLogik.dir/src/variable.c.obj: CMakeFiles/CLogik.dir/flags.make
CMakeFiles/CLogik.dir/src/variable.c.obj: C:/Users/mathf/Desktop/Cours/S1/C/CLogik/src/variable.c
CMakeFiles/CLogik.dir/src/variable.c.obj: CMakeFiles/CLogik.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/CLogik.dir/src/variable.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/CLogik.dir/src/variable.c.obj -MF CMakeFiles\CLogik.dir\src\variable.c.obj.d -o CMakeFiles\CLogik.dir\src\variable.c.obj -c C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\variable.c

CMakeFiles/CLogik.dir/src/variable.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/CLogik.dir/src/variable.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\variable.c > CMakeFiles\CLogik.dir\src\variable.c.i

CMakeFiles/CLogik.dir/src/variable.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/CLogik.dir/src/variable.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\mathf\Desktop\Cours\S1\C\CLogik\src\variable.c -o CMakeFiles\CLogik.dir\src\variable.c.s

# Object files for target CLogik
CLogik_OBJECTS = \
"CMakeFiles/CLogik.dir/src/main.c.obj" \
"CMakeFiles/CLogik.dir/src/lexer.c.obj" \
"CMakeFiles/CLogik.dir/src/token.c.obj" \
"CMakeFiles/CLogik.dir/src/parser.c.obj" \
"CMakeFiles/CLogik.dir/src/variable.c.obj"

# External object files for target CLogik
CLogik_EXTERNAL_OBJECTS =

CLogik.exe: CMakeFiles/CLogik.dir/src/main.c.obj
CLogik.exe: CMakeFiles/CLogik.dir/src/lexer.c.obj
CLogik.exe: CMakeFiles/CLogik.dir/src/token.c.obj
CLogik.exe: CMakeFiles/CLogik.dir/src/parser.c.obj
CLogik.exe: CMakeFiles/CLogik.dir/src/variable.c.obj
CLogik.exe: CMakeFiles/CLogik.dir/build.make
CLogik.exe: CMakeFiles/CLogik.dir/linkLibs.rsp
CLogik.exe: CMakeFiles/CLogik.dir/objects1.rsp
CLogik.exe: CMakeFiles/CLogik.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable CLogik.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\CLogik.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CLogik.dir/build: CLogik.exe
.PHONY : CMakeFiles/CLogik.dir/build

CMakeFiles/CLogik.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\CLogik.dir\cmake_clean.cmake
.PHONY : CMakeFiles/CLogik.dir/clean

CMakeFiles/CLogik.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\mathf\Desktop\Cours\S1\C\CLogik C:\Users\mathf\Desktop\Cours\S1\C\CLogik C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug C:\Users\mathf\Desktop\Cours\S1\C\CLogik\cmake-build-debug\CMakeFiles\CLogik.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/CLogik.dir/depend

