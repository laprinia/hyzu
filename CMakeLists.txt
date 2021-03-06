
cmake_minimum_required(VERSION 3.17)
project(hyzu)
set(CMAKE_CXX_STANDARD 11)

set(IMGUI_PATH  ${PROJECT_SOURCE_DIR}/libs/imgui)
if (NOT WIN32)
    set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
    set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)
endif (NOT WIN32)

find_package (OpenGL REQUIRED)
if (WIN32)
    #imgui
    file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp)
    add_library("ImGui" STATIC ${IMGUI_SOURCES})
    target_include_directories("ImGui" PUBLIC ${IMGUI_PATH})
    #imgui glfw

    include_directories( ${PROJECT_SOURCE_DIR}/libs/GLFW/include ${PROJECT_SOURCE_DIR}/libs/GLEW/include "${PROJECT_SOURCE_DIR}/libs/Assimp/include" "${PROJECT_SOURCE_DIR}/libs/SOIL/include" "C:/glm" ${PROJECT_SOURCE_DIR}/libs/imgui)
    link_directories("${PROJECT_SOURCE_DIR}/libs/GLEW/lib/Release/x64"  "${PROJECT_SOURCE_DIR}/libs/GLFW/lib-mingw" "${PROJECT_SOURCE_DIR}/libs/Assimp/lib" "${PROJECT_SOURCE_DIR}/libs/SOIL" )

    set(BUILD_ASSIMP_TOOLS  ON)
    set(ASSIMP_BUILD_STATIC_LIB ON)
else (WIN32)
    find_package (glfw3 REQUIRED)
    find_package (GLM REQUIRED)
    find_package (GLEW REQUIRED STATIC)
    find_library(SOIL SOIL lib/libsoil.a)
endif (WIN32)
if (APPLE)
    include_directories(/usr/local/include)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVID_LIBRARY CoreVideo REQUIRED)
endif (APPLE)

set(SOURCE_FILES main.cpp input/InputManager.cpp input/InputManager.h window/SampleWindow.cpp window/SampleWindow.h render/RenderableWithVertexColor.cpp render/RenderableWithVertexColor.h shaders/ShaderManager.cpp shaders/ShaderManager.h render/RenderableObject.h camera/Camera.cpp camera/Camera.h render/Mesh.cpp render/Mesh.h render/SimpleMesh.cpp render/SimpleMesh.h render/Model.cpp render/Model.h import/AssimpImport.cpp import/AssimpImport.h)

## "hyzu" is the name of the executable
add_executable(hyzu ${SOURCE_FILES})
target_link_libraries (hyzu
        ${GLFW3_LIBRARY}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARY}
        ${COCOA_LIBRARY} ${COREVID_LIBRARY} ${IOKIT_LIBRARY} ${SOIL} ImGui)
if (WIN32)
    target_link_libraries (hyzu ImGui assimp  ${OPENGL_LIBRARIES} glfw3 glu32 glew32s SOIL opengl32 )

endif (WIN32)

if (UNIX)
    target_link_libraries (hyzu
            ${OPENGL_LIBRARIES}
            ${GLFW3_LIBRARY}
            ${GLEW_LIBRARY}
            Xxf86vm pthread Xrandr Xinerama Xi Xcursor)
endif (UNIX)