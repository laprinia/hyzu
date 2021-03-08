//
// Created by Lavinia on 3/1/2021.
//

#ifndef HYZU_SAMPLEWINDOW_H
#define HYZU_SAMPLEWINDOW_H
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../input/InputManager.h"
#include <iostream>

class SampleWindow {
private:
    InputManager inputManager = InputManager::GetInstance();
    GLFWwindow *window;
    int width, height;
public:
    SampleWindow(int width, int height, std::string title);

    ~SampleWindow();

    GLFWwindow *GetWindowContext();

    int GetWindowHeight();

    int GetWindowWidth();

};
#endif //HYZU_SAMPLEWINDOW_H
