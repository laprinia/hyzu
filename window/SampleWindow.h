

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
    glm::vec3 diffuseColor = glm::vec3(1.0f, 0.6196078f, 0.452941176f);
    glm::vec3 specularColor = glm::vec3(1.0f, 0.5372549f, 0.8117647f);
    glm::vec3 position = glm::vec3(-48, 31, -83);
    glm::vec3 target = glm::vec3(0, 0, 0);

    glm::vec3 GetDirection() const {
        return -(glm::normalize(position - target));
    }

    float lightExposure = 1.1f;
};


struct PointLight {
    glm::vec3 diffuseColor = glm::vec3(0.650980f, 0.1254902f, 0.8980392f);
    glm::vec3 specularColor = glm::vec3(1.0f, 0.16470f, 0.4666667f);
    glm::vec3 position = glm::vec3(1.0f, 16.0f, -38.0f);

    float constant = 0.20f;
    float linear = 0.030f;
    float quadratic = 0.001f;
};
struct SpotLight {
    glm::vec3 position=glm::vec3(23, 30, 41);
    glm::vec3 target = glm::vec3(24, 14, 4);
    glm::vec3 GetDirection() const {
        return (glm::normalize(position - target));
    }
    float outterCutAngle = 14;
    float GetOutterCut()  {
        return glm::cos(glm::radians(outterCutAngle));
    }
    float cutAngle = 13.5;
    float GetCut() {
        return glm::cos(glm::radians(12.5f));
    }

    glm::vec3 diffuseColor = glm::vec3(1.0f, 0.6196078f, 0.32941f);
    glm::vec3 specularColor = glm::vec3(1.0f, 0.780392f, 0.643137f);

    float constant = 0.20f;
    float linear = 0.030f;
    float quadratic = 0.001f;
};

class SampleWindow {
private:
    static Camera *camera;
    GLFWwindow *window;
    const bool hasGUI = false;
    unsigned int *fbID;
    unsigned int *depthID;
    unsigned int *occID;
    unsigned int *bufferTexture;
    unsigned int *depthTexture;
    unsigned int *occTexture;
    float density=1.20f;
    float weight=0.01f;
    float decay=1.0f;
    float exposure=1.2f;
    int samples=270;
    const unsigned int depth_width_height = 1024;
    float nearPlane = 40.0f, farPlane = 800.0f;
    float lightAngle = 35.9f;
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
    std::unordered_map<std::string, RenderableObject*> renderables;
    std::unordered_map<std::string, Model*> models;
    std::unordered_map<std::string, GLuint> shaders;
    DirectionalLight directional;
    PointLight point,point2;
    SpotLight spot, spot2;

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

    void RenderModel(const std::string &modelName, glm::mat4 &modelMatrix, glm::mat4 &viewMatrix,
                     glm::mat4 &projectionMatrix, glm::mat4 &lightMatrix, GLuint shaderProgram);


    void SendLightingDataToShader(GLuint shaderProgram);

    void SendPostDataToShader(GLuint shaderProgram);

    unsigned int LoadCubeMap(const std::vector<std::string> &faceLocations);

    void InitCubeMap();

    void OnInputUpdate();

    void GUIUpdate();

    void RenderScene(GLuint  shader,glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, bool isDepthPass, glm::mat4 &lightMatrix);

    void RenderSun(GLuint shader, glm::mat4 &modelMatrix, glm::mat4 &viewMatrix,
                   glm::mat4 &projectionMatrix, glm::mat4 &lightMatrix);

    glm::vec2 GetSunScreenPosition(const glm::mat4& viewMatrix,const glm::mat4& projectionMatrix) ;
};

#endif //HYZU_SAMPLEWINDOW_H
