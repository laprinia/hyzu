//
// Created by Lavinia on 3/1/2021.
//

#include "InputManager.h"

InputManager::InputManager() = default;

GLFWwindow *InputManager::windowContext = nullptr;

void InputManager::SetWindowContext(GLFWwindow *window) {
    InputManager::windowContext = window;
    glfwSetKeyCallback(window, OnKeyPress);
}

void InputManager::OnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

}

InputManager &InputManager::GetInstance() {
    static InputManager instance;
    return instance;
}





