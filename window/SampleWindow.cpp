//
// Created by Lavinia on 3/1/2021.
//

#include "SampleWindow.h"

SampleWindow::SampleWindow(int width, int height, std::string title) {

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
    InputManager::SetWindowContext(window);
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }
    glViewport(0, 0, 800, 600);
    SampleWindow::CompileShaders();
    SampleWindow::renderableObject.RenderObject();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.14f, 0.13f, 0.21f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        GLfloat timeValue=glfwGetTime();
        GLfloat greenValue=sin(timeValue)/2+0.5;
        glUseProgram(ShaderManager::shaderProgram);
        glUniform4f(glGetUniformLocation(ShaderManager::shaderProgram,"uniformColor"),0.0f,greenValue,0.0f,1.0f);

        glBindVertexArray(renderableObject.VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

void SampleWindow::CompileShaders() {
    ShaderManager::CompileShader(GL_VERTEX_SHADER, "../shaders/BaseVS.glsl");
    ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

    ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "../shaders/BaseFS.glsl");
    ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

    ShaderManager::LinkShaderProgram();
    ShaderManager::CheckShaderLink();
}


SampleWindow::~SampleWindow() {

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




