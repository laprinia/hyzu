# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\Projects\hyzu

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\Projects\hyzu\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ImGui.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ImGui.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ImGui.dir/flags.make

CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.obj: CMakeFiles/ImGui.dir/flags.make
CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.obj: CMakeFiles/ImGui.dir/includes_CXX.rsp
CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.obj: ../libs/imgui/imgui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Projects\hyzu\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\libs\imgui\imgui.cpp.obj -c E:\Projects\hyzu\libs\imgui\imgui.cpp

CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Projects\hyzu\libs\imgui\imgui.cpp > CMakeFiles\ImGui.dir\libs\imgui\imgui.cpp.i

CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\Projects\hyzu\libs\imgui\imgui.cpp -o CMakeFiles\ImGui.dir\libs\imgui\imgui.cpp.s

CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.obj: CMakeFiles/ImGui.dir/flags.make
CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.obj: CMakeFiles/ImGui.dir/includes_CXX.rsp
CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.obj: ../libs/imgui/imgui_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Projects\hyzu\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\libs\imgui\imgui_demo.cpp.obj -c E:\Projects\hyzu\libs\imgui\imgui_demo.cpp

CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Projects\hyzu\libs\imgui\imgui_demo.cpp > CMakeFiles\ImGui.dir\libs\imgui\imgui_demo.cpp.i

CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\Projects\hyzu\libs\imgui\imgui_demo.cpp -o CMakeFiles\ImGui.dir\libs\imgui\imgui_demo.cpp.s

CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.obj: CMakeFiles/ImGui.dir/flags.make
CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.obj: CMakeFiles/ImGui.dir/includes_CXX.rsp
CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.obj: ../libs/imgui/imgui_draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Projects\hyzu\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\libs\imgui\imgui_draw.cpp.obj -c E:\Projects\hyzu\libs\imgui\imgui_draw.cpp

CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Projects\hyzu\libs\imgui\imgui_draw.cpp > CMakeFiles\ImGui.dir\libs\imgui\imgui_draw.cpp.i

CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\Projects\hyzu\libs\imgui\imgui_draw.cpp -o CMakeFiles\ImGui.dir\libs\imgui\imgui_draw.cpp.s

CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.obj: CMakeFiles/ImGui.dir/flags.make
CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.obj: CMakeFiles/ImGui.dir/includes_CXX.rsp
CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.obj: ../libs/imgui/imgui_impl_glfw_gl3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Projects\hyzu\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.obj"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ImGui.dir\libs\imgui\imgui_impl_glfw_gl3.cpp.obj -c E:\Projects\hyzu\libs\imgui\imgui_impl_glfw_gl3.cpp

CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\Projects\hyzu\libs\imgui\imgui_impl_glfw_gl3.cpp > CMakeFiles\ImGui.dir\libs\imgui\imgui_impl_glfw_gl3.cpp.i

CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\Projects\hyzu\libs\imgui\imgui_impl_glfw_gl3.cpp -o CMakeFiles\ImGui.dir\libs\imgui\imgui_impl_glfw_gl3.cpp.s

# Object files for target ImGui
ImGui_OBJECTS = \
"CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.obj" \
"CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.obj" \
"CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.obj" \
"CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.obj"

# External object files for target ImGui
ImGui_EXTERNAL_OBJECTS =

libImGui.a: CMakeFiles/ImGui.dir/libs/imgui/imgui.cpp.obj
libImGui.a: CMakeFiles/ImGui.dir/libs/imgui/imgui_demo.cpp.obj
libImGui.a: CMakeFiles/ImGui.dir/libs/imgui/imgui_draw.cpp.obj
libImGui.a: CMakeFiles/ImGui.dir/libs/imgui/imgui_impl_glfw_gl3.cpp.obj
libImGui.a: CMakeFiles/ImGui.dir/build.make
libImGui.a: CMakeFiles/ImGui.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\Projects\hyzu\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libImGui.a"
	$(CMAKE_COMMAND) -P CMakeFiles\ImGui.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ImGui.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ImGui.dir/build: libImGui.a

.PHONY : CMakeFiles/ImGui.dir/build

CMakeFiles/ImGui.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ImGui.dir\cmake_clean.cmake
.PHONY : CMakeFiles/ImGui.dir/clean

CMakeFiles/ImGui.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\Projects\hyzu E:\Projects\hyzu E:\Projects\hyzu\cmake-build-debug E:\Projects\hyzu\cmake-build-debug E:\Projects\hyzu\cmake-build-debug\CMakeFiles\ImGui.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ImGui.dir/depend

