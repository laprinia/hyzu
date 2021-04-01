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
#include <iostream>
#include <vector>
#include <unordered_map>

class SampleWindow {
private:
    InputManager inputManager = InputManager::GetInstance();
    ShaderManager shaderManager = ShaderManager::GetInstance();

    GLFWwindow *window;
    int width, height;
    std::unordered_map<std::string, RenderableObject*> renderables;
    static void CompileShaders();


public:
    SampleWindow(int width, int height, std::string title);

    ~SampleWindow();

    void Update();
    void Init();
    void RenderMeshFromData(const std::string& meshName, const glm::mat4& modelMatrix );
    GLFWwindow *GetWindowContext();

    int GetWindowHeight();

    int GetWindowWidth();

    void RenderMeshFromData(const std::string &meshName, glm::mat4 &modelMatrix);
};

#endif //HYZU_SAMPLEWINDOW_H
