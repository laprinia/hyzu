//
// Created by Lavinia on 3/1/2021.
//

#include "InputManager.h"

InputManager::InputManager() = default;

GLFWwindow *InputManager::windowContext = nullptr;

InputManager &InputManager::GetInstance() {
    static InputManager instance;
    return instance;
}

void InputManager::SetWindowContext(GLFWwindow *window,void (*function)(GLFWwindow*,int,int,int,int)) {
    InputManager::windowContext = window;
    glfwSetKeyCallback(window, function);
}







