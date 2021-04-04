
cmake_minimum_required(VERSION 3.17)
project(hyzu)
set(CMAKE_CXX_STANDARD 11)

if (NOT WIN32)
    set(CMAKE_MODULE_PATH /usr/local/lib/cmake /usr/local/lib/x86_64-linux-gnu/cmake)
    set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw)
endif (NOT WIN32)


find_package (OpenGL REQUIRED)
if (WIN32)

    include_directories( ${PROJECT_SOURCE_DIR}/GLFW/include ${PROJECT_SOURCE_DIR}/GLEW/include  "C:/glm")
    link_directories("${PROJECT_SOURCE_DIR}/GLEW/lib/Release/Win32" "${PROJECT_SOURCE_DIR}/GLFW/lib-mingw" )
else (WIN32)
    find_package (glfw3 REQUIRED)
    find_package (GLM REQUIRED)
    find_package (GLEW REQUIRED STATIC)
endif (WIN32)
if (APPLE)
    include_directories(/usr/local/include)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVID_LIBRARY CoreVideo REQUIRED)
endif (APPLE)

set(SOURCE_FILES main.cpp input/InputManager.cpp input/InputManager.h window/SampleWindow.cpp window/SampleWindow.h render/RenderableWithVertexColor.cpp render/RenderableWithVertexColor.h shaders/ShaderManager.cpp shaders/ShaderManager.h render/RenderableObject.h camera/Camera.cpp camera/Camera.h)

## "hyzu" is the name of the executable
add_executable(hyzu ${SOURCE_FILES})
target_link_libraries (hyzu
        ${GLFW3_LIBRARY}
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARY}
        ${COCOA_LIBRARY} ${COREVID_LIBRARY} ${IOKIT_LIBRARY})
if (WIN32)
    target_link_libraries (hyzu
            ${OPENGL_LIBRARIES} glfw3 glew32s glu32 opengl32)
endif (WIN32)

if (UNIX)
    target_link_libraries (hyzu
            ${OPENGL_LIBRARIES}
            ${GLFW3_LIBRARY}
            ${GLEW_LIBRARY}
            Xxf86vm pthread Xrandr Xinerama Xi Xcursor)
endif (UNIX)