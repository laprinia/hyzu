//
// Created by Lavinia on 3/1/2021.
//

#include "SampleWindow.h"

Camera *SampleWindow::camera = nullptr;
float SampleWindow::cameraSpeed = 20.0f;
float SampleWindow::deltaTime = 0.0f;
float SampleWindow::lastFrame = 0.0f;
bool SampleWindow::firstMouseMove = true;
double SampleWindow::lastMouseX = 0.0f;
double SampleWindow::lastMouseY = 0.0f;
double SampleWindow::yaw = -90.0f;
double SampleWindow::pitch = 0.0f;

float SampleWindow::mouseSensitivity = 0.1f;

bool show_test_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImColor(114, 144, 154);

SampleWindow::SampleWindow(int width, int height, const std::string &title) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 16);
    this->width = width;
    this->height = height;
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    GLFWimage icon;
    icon.pixels = SOIL_load_image("../resources/icon_dark.png", &icon.width, &icon.height, 0, SOIL_LOAD_RGBA);
    glfwSetWindowIcon(window, 1, &icon);
    SOIL_free_image_data(icon.pixels);
    glewExperimental = GL_TRUE;
    InputManager::GetInstance(window);
    InputManager::SetFramebufferSizeCallback(window, OnFramebufferSizeChange);
    InputManager::SetWindowKeyCallback(window, OnKeyPress);
    InputManager::SetWindowCursorPositionCallback(window, OnCursorPositionChange);
    InputManager::SetWindowScrollCallback(window, OnScrollChange);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }
    glViewport(0, 0, width, height);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    SampleWindow::CompileShaders();
    SampleWindow::Init();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    unsigned int fb;
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    fbID = &fb;

    unsigned int bt;
    glGenTextures(1, &bt);
    glBindTexture(GL_TEXTURE_2D, bt);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bt, 0);
    bufferTexture = &bt;

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << " Framebuffer isn't complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window)) {

        SampleWindow::Update();
        glfwSwapBuffers(window);
    }
    if (hasGUI) ImGui_ImplGlfwGL3_Shutdown();
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1,&cubeVBO);

    glfwTerminate();
}

void SampleWindow::Init() {

    if (hasGUI) {
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
    }
    float screenQuadVertices[] = {

            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    camera = new Camera(glm::vec3(0.0f, 10.0f, 4.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    Model *model1 = new Model("../resources/scenes/pool/pool2.obj");
    models["env"] = model1;
    Model *model2 = new Model("../resources/scenes/pool/pool3.obj");
    models["env2"] = model2;

    Model *model3 = new Model("../resources/models/bulb/sphere.obj");
    models["bulb"] = model3;

    SampleWindow::InitCubeMap();

}

void SampleWindow::Update() {
    if (hasGUI) ImGui_ImplGlfwGL3_NewFrame();
    glfwPollEvents();
    OnInputUpdate();
    if (hasGUI) GUIUpdate();

    glBindFramebuffer(GL_FRAMEBUFFER, *fbID);

    glClearColor(0.14f, 0.13f, 0.21f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //ENV

    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaders["env"]);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    SampleWindow::RenderModel("env", model, shaders["env"]);
    SampleWindow::SendLightingDataToShader(shaders["env"]);
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA );
    SampleWindow::RenderModel("env2", model, shaders["env"]);
    SampleWindow::SendLightingDataToShader(shaders["env"]);


    glDisable(GL_BLEND);
    glUseProgram(shaders["base"]);
    model = glm::mat4(1.0f);

    model = glm::translate(model, point.position);
    SampleWindow::RenderModel("bulb", model, shaders["base"]);
    SampleWindow::SendLightingDataToShader(shaders["env"]);


    //skybox
    glDepthFunc(GL_LEQUAL);
    glUseProgram(shaders["skybox"]);
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(400.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraFront(),camera->getCameraUp());
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera->getFieldOfView()), (float) width / (float) height, 0.1f, 300.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaders["skybox"], "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaders["skybox"], "projection"), 1, GL_FALSE,glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaders["skybox"], "model"), 1, GL_FALSE,glm::value_ptr(model));

    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);


    //post
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaders["post"]);
    SampleWindow::SendPostDataToShader(shaders["post"]);
    glBindVertexArray(quadVAO);
    glBindTexture(GL_TEXTURE_2D, *bufferTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);



    if (hasGUI) ImGui::Render();
}

void SampleWindow::GUIUpdate() {

    {
        ImGui::Text("Exposure");
        ImGui::DragFloat("Light Exposure", (float *) &directional.lightExposure, 0.10f, 0.1f, 5.0f);
        ImGui::Text("Directional variables");
        ImGui::DragFloat3("Light Direction", (float *) &directional.direction);
        ImGui::ColorEdit3("Diffuse Light Color", (float *) &directional.diffuseColor);
        ImGui::ColorEdit3("Specular Light Color", (float *) &directional.specularColor);
        ImGui::Text("Point variables");
        ImGui::DragFloat3("Light Position", (float *) &point.position);
        ImGui::ColorEdit3(" Diffuse Light Color", (float *) &point.diffuseColor);
        ImGui::ColorEdit3(" Specular Light Color", (float *) &point.specularColor);
        ImGui::DragFloat("Constant", (float *) &point.constant, 0.03f, 0.0f, 1.0f);
        ImGui::DragFloat("Linear", (float *) &point.linear, 0.03f, 0.0f, 1.0f);
        ImGui::DragFloat("Quadratic", (float *) &point.quadratic, 0.03f, 0.0f, 1.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
    }

}

void SampleWindow::SendPostDataToShader(GLuint shaderProgram) {

    glUniform1f(glGetUniformLocation(shaderProgram, "lightExposure"), directional.lightExposure);
}

void SampleWindow::SendLightingDataToShader(GLuint shaderProgram) {

    glUniform3fv(glGetUniformLocation(shaderProgram, "directLightColor"), 1, glm::value_ptr(directional.diffuseColor));
    glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight"), 1, glm::value_ptr(directional.direction));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPosition"), 1, glm::value_ptr(camera->getCameraPosition()));

    //directional
    glUniform3fv(glGetUniformLocation(shaderProgram, "directional.direction"), 1,
                 glm::value_ptr(directional.direction));
    glUniform3fv(glGetUniformLocation(shaderProgram, "directional.ambient"), 1,
                 glm::value_ptr(directional.diffuseColor));
    glUniform3fv(glGetUniformLocation(shaderProgram, "directional.diffuse"), 1,
                 glm::value_ptr(directional.diffuseColor));
    glUniform3fv(glGetUniformLocation(shaderProgram, "directional.specular"), 1,
                 glm::value_ptr(directional.specularColor));

    //point
    glUniform3fv(glGetUniformLocation(shaderProgram, "point.position"), 1, glm::value_ptr(point.position));
    glUniform3fv(glGetUniformLocation(shaderProgram, "point.ambient"), 1, glm::value_ptr(point.diffuseColor));
    glUniform3fv(glGetUniformLocation(shaderProgram, "point.diffuse"), 1, glm::value_ptr(point.diffuseColor));
    glUniform3fv(glGetUniformLocation(shaderProgram, "point.specular"), 1,
                 glm::value_ptr(point.specularColor));

    glUniform1f(glGetUniformLocation(shaderProgram, "point.constant"), point.constant);
    glUniform1f(glGetUniformLocation(shaderProgram, "point.linear"), point.linear);
    glUniform1f(glGetUniformLocation(shaderProgram, "point.quadratic"), point.quadratic);
}

void SampleWindow::RenderModel(const std::string &modelName, glm::mat4 &modelMatrix, GLuint shaderProgram) {

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraFront(),
                       camera->getCameraUp());

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera->getFieldOfView()), (float) width / (float) height, 0.1f, 300.0f);


    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));

    models[modelName]->Draw(shaderProgram);
}

void SampleWindow::RenderMeshFromData(const std::string &meshName, glm::mat4 &modelMatrix, GLuint shaderProgram) {

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraFront(),
                       camera->getCameraUp());

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera->getFieldOfView()), (float) width / (float) height, 0.1f, 300.0f);


    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
                       glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE,
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

    GLuint shaderProgram = ShaderManager::LinkShaderProgram();
    shaders["base"] = shaderProgram;
    ShaderManager::CheckShaderLink(shaders["base"]);

    ShaderManager::CompileShader(GL_VERTEX_SHADER, "../shaders/EnvVS.glsl");
    ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

    ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "../shaders/EnvFS.glsl");
    ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

    shaderProgram = ShaderManager::LinkShaderProgram();
    shaders["env"] = shaderProgram;
    ShaderManager::CheckShaderLink(shaders["env"]);

    ShaderManager::CompileShader(GL_VERTEX_SHADER, "../shaders/PostVS.glsl");
    ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

    ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "../shaders/PostFS.glsl");
    ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

    shaderProgram = ShaderManager::LinkShaderProgram();
    shaders["post"] = shaderProgram;
    ShaderManager::CheckShaderLink(shaders["post"]);

    ShaderManager::CompileShader(GL_VERTEX_SHADER, "../shaders/SkyVS.glsl");
    ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

    ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "../shaders/SkyFS.glsl");
    ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

    shaderProgram = ShaderManager::LinkShaderProgram();
    shaders["skybox"] = shaderProgram;
    ShaderManager::CheckShaderLink(shaders["skybox"]);
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
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera->setCameraPosition(camera->getCameraPosition() + (actualSpeed * camera->getCameraUp()));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera->setCameraPosition(camera->getCameraPosition() - (actualSpeed * camera->getCameraUp()));
    }

}

void SampleWindow::OnFramebufferSizeChange(GLFWwindow *window, int width, int height) {

    glViewport(0, 0, width, height);
}


SampleWindow::~SampleWindow() {
    for (auto const &renderable : renderables) {
        delete renderable.second;
    }
    renderables.clear();
    for (auto const &model : models) {
        delete model.second;
    }
    models.clear();
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

unsigned int SampleWindow::LoadCubeMap(const std::vector<std::string> &faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void SampleWindow::InitCubeMap() {
    float cubeVertices[] = {

            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    std::vector<std::string> cubeMapFaces{
            "../resources/skybox/pinkright.png",
            "../resources/skybox/pinkleft.png",
            "../resources/skybox/pinkup.png",
            "../resources/skybox/pinkbottom.png",
            "../resources/skybox/pinkfront.png",
            "../resources/skybox/pinkback.png",
    };
    cubemapTexture = SampleWindow::LoadCubeMap(cubeMapFaces);
}
