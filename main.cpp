#undef GLFW_DLL
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>


int main() {
    if (!glfwInit()) {
        std::cerr << "Can't initialize GLFW" << std::endl;
        exit (EXIT_FAILURE);
    }

    GLFWwindow *win;
    win = glfwCreateWindow (450, 300, "Hello", NULL, NULL);
    glfwMakeContextCurrent(win);
    while (!glfwWindowShouldClose(win)) {
        glfwWaitEvents();
    }
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}