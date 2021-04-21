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
#include "../render/SimpleMesh.h"
#include "../camera/Camera.h"
#include "../render/Model.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <iostream>
#include <vector>
#include <unordered_map>


class SampleWindow {
private:
    InputManager inputManager = InputManager::GetInstance(nullptr);
    ShaderManager shaderManager = ShaderManager::GetInstance();
    static Camera *camera;
    GLFWwindow *window;
    const bool hasGUI = true;
    int width, height;
    static bool firstMouseMove;
    static double lastMouseX, lastMouseY;
    static double yaw, pitch;
    static float mouseSensitivity;
    static float cameraSpeed;
    static float deltaTime;
    static float lastFrame;
    std::unordered_map<std::string, RenderableObject *> renderables;
    std::unordered_map<std::string, Model *> models;
    std::unordered_map<std::string, GLuint> shaders;
    glm::vec3 directLightColor = glm::vec3(0.9943f, 1.0f, 0.701961);
    glm::vec3 directionalLight = glm::vec3(0, 0, -30);
    glm::vec3 pointLight = glm::vec3(0, 20, -30);
    glm::vec3 pointLightColor = glm::vec3(0.9943f, 1.0f, 0.701961);

    void CompileShaders();

    static void OnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mode);

    static void OnCursorPositionChange(GLFWwindow *window, double xPosition, double yPosition);

    static void OnScrollChange(GLFWwindow *window, double xOffset, double yOffset);

    static void OnFramebufferSizeChange(GLFWwindow *window, int width, int height);


public:
    SampleWindow(int width, int height, const std::string &title);

    ~SampleWindow();

    void Update();

    void Init();

    GLFWwindow *GetWindowContext();

    int GetWindowHeight();

    int GetWindowWidth();

    void RenderMeshFromData(const std::string &meshName, glm::mat4 &modelMatrix, GLuint shaderProgram);

    void RenderModel(const std::string &modelName, glm::mat4 &modelMatrix, GLuint shaderProgram);

    void SendLightingDataToShader(GLuint shaderProgram);

    void OnInputUpdate();

    void GUIUpdate();
};

#endif //HYZU_SAMPLEWINDOW_H
