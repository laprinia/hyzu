

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
#include "../libs/stb/stb_image.h"
#include <iostream>
#include <vector>
#include <unordered_map>

struct DirectionalLight {
    glm::vec3 diffuseColor = glm::vec3(1, 1, 1);
    glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 position = glm::vec3(-76, 64, 24);
    glm::vec3 target =glm::vec3 (0,0,0);
    glm::vec3 GetDirection () const {
        return -(glm::normalize(position-target));
    }
    float lightExposure = 0.1f;
};
struct PointLight {
    glm::vec3 diffuseColor = glm::vec3(1.0f, 0.016471f, 0.036471f);
    glm::vec3 specularColor = glm::vec3(1.0f, 0.16470f, 0.776470f);
    glm::vec3 position = glm::vec3(0, -5.0f, -35.0f);

    float constant = 0.630f;
    float linear = 0.070f;
    float quadratic = 0.017f;
};

class SampleWindow {
private:
    InputManager inputManager = InputManager::GetInstance(nullptr);
    ShaderManager shaderManager = ShaderManager::GetInstance();
    static Camera *camera;
    GLFWwindow *window;
    const bool hasGUI = true;
    unsigned int *fbID;
    unsigned int *depthID;
    unsigned int *bufferTexture;
    unsigned int *depthTexture;
    const unsigned int depth_width_height = 1024;
    float nearPlane = 68.0f, farPlane = 150.0f;
    float lightAngle=45.0f;
    unsigned int quadVAO;
    unsigned int quadVBO;
    unsigned int cubeVAO;
    unsigned int cubeVBO;
    unsigned int cubemapTexture;
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
    DirectionalLight directional;
    PointLight point;

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

    void RenderModel(const std::string &modelName, glm::mat4 &modelMatrix,glm::mat4 &viewMatrix,glm::mat4 &projectionMatrix,glm::mat4 &lightMatrix, GLuint shaderProgram);


    void SendLightingDataToShader(GLuint shaderProgram);

    void SendPostDataToShader(GLuint shaderProgram);

    unsigned int LoadCubeMap(const std::vector<std::string>& faceLocations);

    void InitCubeMap();

    void OnInputUpdate();

    void GUIUpdate();

    void RenderScene(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, bool isDepthPass, glm::mat4 &lightMatrix);
};

#endif //HYZU_SAMPLEWINDOW_H
