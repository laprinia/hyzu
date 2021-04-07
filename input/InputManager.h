//
// Created by Lavinia on 3/1/2021.
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
};
#endif
