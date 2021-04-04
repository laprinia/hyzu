//
// Created by Lavinia on 3/1/2021.
//


#include "SampleWindow.h"

Camera* SampleWindow::camera= nullptr;
float SampleWindow::cameraSpeed=2.0f;
float SampleWindow::deltaTime=0.0f;
float SampleWindow::lastFrame=0.0f;

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
    InputManager::SetWindowContext(window, OnKeyPress);
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }
    glViewport(0, 0, 800, 600);
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
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraFront(),
                       camera->getCameraUp());

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 100.0f);


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
    float currentTime=glfwGetTime();
    deltaTime=currentTime-lastFrame;
    lastFrame=currentTime;
    float actualSpeed=cameraSpeed*deltaTime;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if(key==GLFW_KEY_W && action==GLFW_PRESS){
        camera->setCameraPosition(camera->getCameraPosition()+(actualSpeed*camera->getCameraFront()));
    }
    if(key==GLFW_KEY_S && action==GLFW_PRESS){
        camera->setCameraPosition(camera->getCameraPosition()-(actualSpeed*camera->getCameraFront()));
    }
    if(key==GLFW_KEY_A && action==GLFW_PRESS){
        camera->setCameraPosition(camera->getCameraPosition()-(actualSpeed*(glm::normalize(glm::cross(camera->getCameraFront(),camera->getCameraUp())))));
    }
    if(key==GLFW_KEY_D && action==GLFW_PRESS){
        camera->setCameraPosition(camera->getCameraPosition()+(actualSpeed*(glm::normalize(glm::cross(camera->getCameraFront(),camera->getCameraUp())))));
    }

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








