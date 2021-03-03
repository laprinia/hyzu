//
// Created by Lavinia on 3/1/2021.
//

#ifndef HYZU_INPUTMANAGER_H
#define HYZU_INPUTMANAGER_H
#include <GLFW/glfw3.h>

class InputManager {
private:
    InputManager();

public:
    static GLFWwindow *windowContext;
    static InputManager& GetInstance();
    static void SetWindowContext(GLFWwindow *window);
    void static OnKeyPress(GLFWwindow *key, int scanCode, int action, int mode, int i);

};
#endif
