//
// Created by Lavinia on 3/1/2021.
//

#include "InputManager.h"

InputManager::InputManager(GLFWwindow *windowContext){
    InputManager::windowContext=windowContext;
};

GLFWwindow *InputManager::windowContext = nullptr;

InputManager &InputManager::GetInstance(GLFWwindow *windowContext) {
    static InputManager instance(windowContext);
    return instance;
}

void InputManager::SetWindowKeyCallback(GLFWwindow *window, void (*function)(GLFWwindow*, int, int, int, int)) {
    glfwSetKeyCallback(window, function);
}

void InputManager::SetWindowCursorPositionCallback(GLFWwindow *window, void (*function)(GLFWwindow *, double, double)) {
    glfwSetCursorPosCallback(window,function);
}

void InputManager::SetWindowScrollCallback(GLFWwindow *window, void (*function)(GLFWwindow *, double, double)) {
    glfwSetScrollCallback(window,function);
}

void InputManager::SetFramebufferSizeCallback(GLFWwindow *window, void (*function)(GLFWwindow *, int, int)) {
    glfwSetFramebufferSizeCallback(window,function);
}







