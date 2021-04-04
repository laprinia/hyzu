//
// Created by Lavinia on 3/1/2021.
//

#ifndef HYZU_SAMPLEWINDOW_H
#define HYZU_SAMPLEWINDOW_H
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../input/InputManager.h"
#include "../shaders/ShaderManager.h"
#include "../render/RenderableWithVertexColor.h"
#include "../camera/Camera.h"
#include <iostream>
#include <vector>
#include <unordered_map>

class SampleWindow {
private:
    InputManager inputManager = InputManager::GetInstance();
    ShaderManager shaderManager = ShaderManager::GetInstance();
    static Camera* camera;
    GLFWwindow *window;
    int width, height;
    static float cameraSpeed;
    static float deltaTime;
    static float lastFrame;
    std::unordered_map<std::string, RenderableObject*> renderables;
    static void CompileShaders();
    static void OnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mode);

public:
    SampleWindow(int width, int height, const std::string& title);

    ~SampleWindow();

    void Update();
    void Init();

    GLFWwindow *GetWindowContext();

    int GetWindowHeight();

    int GetWindowWidth();

    void RenderMeshFromData(const std::string &meshName, glm::mat4 &modelMatrix);
};

#endif //HYZU_SAMPLEWINDOW_H
