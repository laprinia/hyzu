//
// Created by Lavinia
//

#ifndef HYZU_INPUTMANAGER_H
#define HYZU_INPUTMANAGER_H
#include <GLFW/glfw3.h>

class InputManager {
private:
    static GLFWwindow *windowContext;
    InputManager()=default;
    InputManager(GLFWwindow *windowContext);

public:

    static InputManager& GetInstance(GLFWwindow *windowContext);
    static void SetWindowKeyCallback(GLFWwindow *window, void (*function)(GLFWwindow *, int, int, int, int));
    static void SetWindowCursorPositionCallback(GLFWwindow *window, void (*function)(GLFWwindow *, double ,double ));
    static void SetWindowScrollCallback(GLFWwindow *window, void (*function)(GLFWwindow *, double ,double ));
    static void SetFramebufferSizeCallback(GLFWwindow *window, void (*function)(GLFWwindow *, int ,int ));

};
#endif
