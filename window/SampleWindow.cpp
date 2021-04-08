//
// Created by Lavinia on 3/1/2021.
//

#include "SampleWindow.h"

Camera *SampleWindow::camera = nullptr;
float SampleWindow::cameraSpeed = 2.0f;
float SampleWindow::deltaTime = 0.0f;
float SampleWindow::lastFrame = 0.0f;
bool SampleWindow::firstMouseMove = true;
double SampleWindow::lastMouseX = 0.0f;
double SampleWindow::lastMouseY = 0.0f;
double SampleWindow::yaw = -90.0f;
double SampleWindow::pitch = 0.0f;

float SampleWindow::mouseSensitivity = 0.1f;

SampleWindow::SampleWindow(int width, int height, const std::string &title) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    this->width = width;
    this->height = height;
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    inputManager = InputManager::GetInstance(window);
    InputManager::SetFramebufferSizeCallback(window,OnFramebufferSizeChange);
    InputManager::SetWindowKeyCallback(window, OnKeyPress);
    InputManager::SetWindowCursorPositionCallback(window, OnCursorPositionChange);
    InputManager::SetWindowScrollCallback(window, OnScrollChange);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }
    glViewport(0, 0, width, height);

    //glEnable(GL_DEPTH_TEST);
    SampleWindow::CompileShaders();
    SampleWindow::Init();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window)) {

        SampleWindow::Update();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

void SampleWindow::Init() {
    std::vector<float> vertices = {

            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    };

    std::vector<unsigned int> indices = {
            0, 1, 3,
            1, 2, 3
    };

    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    RenderableObject *renderableObject = new RenderableWithVertexColor();
    renderableObject->DefineObject(vertices, indices);
    renderables["plane"] = renderableObject;
}

void SampleWindow::Update() {
    glfwPollEvents();
    OnInputUpdate();
    glClearColor(0.14f, 0.13f, 0.21f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(ShaderManager::shaderProgram);
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    SampleWindow::RenderMeshFromData("plane", model);
    model = glm::mat4(1.0f);

}

void SampleWindow::RenderMeshFromData(const std::string &meshName, glm::mat4 &modelMatrix) {

    const float radius = 3.0f;

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraFront(),
                       camera->getCameraUp());

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera->getFieldOfView()), (float) width / (float) height, 0.1f, 100.0f);


    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::shaderProgram, "model"), 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(ShaderManager::shaderProgram, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));

    glBindVertexArray(renderables[meshName]->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void SampleWindow::CompileShaders() {
    ShaderManager::CompileShader(GL_VERTEX_SHADER, "../shaders/BaseVS.glsl");
    ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

    ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "../shaders/BaseFS.glsl");
    ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

    ShaderManager::LinkShaderProgram();
    ShaderManager::CheckShaderLink();
}

void SampleWindow::OnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mode) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

}

void SampleWindow::OnCursorPositionChange(GLFWwindow *window, double xPosition, double yPosition) {
    if (firstMouseMove) {
        firstMouseMove = false;
        lastMouseX = xPosition;
        lastMouseY = yPosition;
    }
    float xOffset = xPosition - lastMouseX;
    float yOffset = lastMouseY - yPosition;
    lastMouseX = xPosition;
    lastMouseY = yPosition;

    yaw += mouseSensitivity * xOffset;
    pitch += mouseSensitivity * yOffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = glm::cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));

    camera->setCameraFront(glm::normalize(front));

}

void SampleWindow::OnScrollChange(GLFWwindow *window, double xOffset, double yOffset) {
    camera->setFieldOfView(camera->getFieldOfView() - yOffset);
    float foV = camera->getFieldOfView();
    if (foV < 1.0f) camera->setFieldOfView(1.0f);
    if (foV > 45.0f) camera->setFieldOfView(45.0f);
}

void SampleWindow::OnInputUpdate() {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastFrame;
    lastFrame = currentTime;
    float actualSpeed = cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->setCameraPosition(camera->getCameraPosition() + (actualSpeed * camera->getCameraFront()));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->setCameraPosition(camera->getCameraPosition() - (actualSpeed * camera->getCameraFront()));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->setCameraPosition(camera->getCameraPosition() - (actualSpeed * (glm::normalize(
                glm::cross(camera->getCameraFront(), camera->getCameraUp())))));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->setCameraPosition(camera->getCameraPosition() + (actualSpeed * (glm::normalize(
                glm::cross(camera->getCameraFront(), camera->getCameraUp())))));
    }
}
void SampleWindow::OnFramebufferSizeChange(GLFWwindow *window, int width, int height) {

        glViewport(0,0,width,height);
}


SampleWindow::~SampleWindow() {
    for (auto const &renderable : renderables) {
        delete renderable.second;
    }
    renderables.clear();
    delete camera;
}

GLFWwindow *SampleWindow::GetWindowContext() {
    return window;
}

int SampleWindow::GetWindowHeight() {
    return height;
}

int SampleWindow::GetWindowWidth() {
    return width;
}













