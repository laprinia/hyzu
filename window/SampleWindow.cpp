//
// Created by Lavinia on 3/1/2021.
//

#include "SampleWindow.h"

SampleWindow::SampleWindow(int width, int height, std::string title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    this->width = width;
    this->height = height;
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    inputManager.SetWindowContext(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }
    glViewport(0, 0, 800, 600);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

SampleWindow::~SampleWindow() {

}

GLFWwindow *SampleWindow::GetWindowContext() {
    return window;
}

int SampleWindow::GetWindowHeight() {
    return height;
}

int SampleWindow::GetWindowWidth() {
    return width;
}


