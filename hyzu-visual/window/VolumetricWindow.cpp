//
// Created by Lavinia
//

#include "VolumetricWindow.h"

Camera* VolumetricWindow::camera = nullptr;
float VolumetricWindow::cameraSpeed = 20.0f;
float VolumetricWindow::deltaTime = 0.0f;
float VolumetricWindow::lastFrame = 0.0f;
bool VolumetricWindow::firstMouseMove = true;
double VolumetricWindow::lastMouseX = 0.0f;
double VolumetricWindow::lastMouseY = 0.0f;
double VolumetricWindow::yaw = -90.0f;
double VolumetricWindow::pitch = 0.0f;
float VolumetricWindow::mouseSensitivity = 0.1f;

VolumetricWindow::VolumetricWindow(int width, int height, const std::string& title) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 32);
	this->width = width;
	this->height = height;
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create the GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	GLFWimage icon;
	icon.pixels = SOIL_load_image("../../resources/icon_dark.png", &icon.width, &icon.height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(window, 1, &icon);
	SOIL_free_image_data(icon.pixels);
	glewExperimental = GL_TRUE;
	InputManager::GetInstance(window);
	InputManager::GetInstance(window);
	InputManager::SetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	InputManager::SetWindowKeyCallback(window, OnKeyPress);
	InputManager::SetWindowCursorPositionCallback(window, OnCursorPositionChange);
	InputManager::SetWindowScrollCallback(window, OnScrollChange);
	if (!hasGUI) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
	glViewport(0, 0, width, height);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	VolumetricWindow::CompileShaders();
	VolumetricWindow::Init();

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

	//depth buffer
	unsigned int depthFb;
	glGenFramebuffers(1, &depthFb);

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depth_width_height, depth_width_height, 0, GL_DEPTH_COMPONENT,
		GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindFramebuffer(GL_FRAMEBUFFER, depthFb);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	depthID = &depthFb;
	depthTexture = &depthMap;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//occlusion buffer
	unsigned int occ;
	glGenFramebuffers(1, &occ);
	glBindFramebuffer(GL_FRAMEBUFFER, occ);
	occID = &occ;

	unsigned int occMap;
	glGenTextures(1, &occMap);
	glBindTexture(GL_TEXTURE_2D, occMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, occMap, 0);
	occTexture = &occMap;

	unsigned int rbo2;
	glGenRenderbuffers(1, &rbo2);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo2);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo2);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Occlusion Framebuffer isn't complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window)) {

		VolumetricWindow::Update();
		glfwSwapBuffers(window);
	}
	if (hasGUI) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	glfwTerminate();
}

void VolumetricWindow::Init() {


	if (hasGUI) {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
		ImGui::StyleColorsDark();
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	camera = new Camera(glm::vec3(0.0f, 5.0f, 44.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	Model* model1 = new Model("C:/Users/Laprinia/Desktop/Motoare/hyzu/hyzu-visual/resources/scenes/room/4/bath.obj");
	models["basescene"] = model1;
	Model* model2 = new Model("C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/resources/scenes/pool/water/poolwater.obj");
	models["poolwater"] = model2;

	Model* model3 = new Model("C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/resources/scenes/pool/sphere.obj");
	models["sphere"] = model3;

	VolumetricWindow::InitCubeMap();

	point2.position = glm::vec3(1.0f, 16.0f, 38.0f);

	spot2.position = glm::vec3(26, 30, -27);
	spot2.target = glm::vec3(-11, 2, 15);
	spot2.diffuseColor = glm::vec3(1.0f, 0.2392f, 0.50588f);
	spot2.specularColor = glm::vec3(1.0f, 0.388235f, 0.60392f);
}

void VolumetricWindow::RenderScene(GLuint  shader, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, bool isDepthPass,
	glm::mat4& lightMatrix) {


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	VolumetricWindow::RenderModel("basescene", model, viewMatrix, projectionMatrix, lightMatrix,
		isDepthPass ? shaders["depth"] : shader);
	VolumetricWindow::SendLightingDataToShader(isDepthPass ? shaders["depth"] : shader);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
	VolumetricWindow::RenderModel("poolwater", model, viewMatrix, projectionMatrix, lightMatrix,
		isDepthPass ? shaders["depth"] : shader);
	VolumetricWindow::SendLightingDataToShader(isDepthPass ? shaders["depth"] : shader);
	glDisable(GL_BLEND);*/

}

void VolumetricWindow::Update() {

	glfwPollEvents();
	OnInputUpdate();
	if (hasGUI) GUIUpdate();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//occlusion pass
	glBindFramebuffer(GL_FRAMEBUFFER, *occID);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaders["base"]);

	// matrices
	glm::mat4 lightProjection = glm::perspective(glm::radians(lightAngle),
		(GLfloat)depth_width_height / depth_width_height, nearPlane,
		farPlane);

	glm::mat4 lightView = glm::lookAt(directional.position, directional.target,
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraFront(),
		camera->getCameraUp());
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera->getFieldOfView()), (float)width / (float)height, 0.1f, 800.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, directional.position);
	model = glm::scale(model, glm::vec3(20.0f));

	glm::mat4 empty = glm::mat4(0.0f);
	glUniform1i(glGetUniformLocation(shaders["base"], "isTransparent"), 1);
	glUniform3fv(glGetUniformLocation(shaders["base"], "solidColor"), 1, glm::value_ptr(directional.diffuseColor));
	VolumetricWindow::RenderSun(shaders["base"], model, view, projection, empty);

	glUniform1i(glGetUniformLocation(shaders["base"], "isTransparent"), 0);
	glUniform3fv(glGetUniformLocation(shaders["base"], "solidColor"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
	VolumetricWindow::RenderScene(shaders["base"], view, projection, false, empty);


	//depth pass
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shaders["depth"]);
	glViewport(0, 0, depth_width_height, depth_width_height);
	glBindFramebuffer(GL_FRAMEBUFFER, *depthID);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 bogus = glm::mat4(0);
	VolumetricWindow::RenderScene(shaders["basescene"], bogus, bogus, true, lightSpaceMatrix);


	//normal pass
	glBindFramebuffer(GL_FRAMEBUFFER, *fbID);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//light
	glUseProgram(shaders["base"]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, directional.position);
	model = glm::scale(model, glm::vec3(20.0f));
	glUniform1i(glGetUniformLocation(shaders["base"], "isTransparent"), 1);
	glUniform3fv(glGetUniformLocation(shaders["base"], "solidColor"), 1, glm::value_ptr(directional.diffuseColor));
	VolumetricWindow::RenderSun(shaders["base"], model, view, projection, lightSpaceMatrix);

	//scene
	glUseProgram(shaders["basescene"]);

	glEnable(GL_DEPTH_TEST);
	view = glm::mat4(1.0f);
	view = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraFront(),
		camera->getCameraUp());
	projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera->getFieldOfView()), (float)width / (float)height, 0.1f, 800.0f);

	glUniform1i(glGetUniformLocation(shaders["basescene"], "shadowMap"), 3);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, *depthTexture);

	VolumetricWindow::RenderScene(shaders["basescene"], view, projection, false, lightSpaceMatrix);

	//skybox
	glDepthFunc(GL_LEQUAL);
	glUseProgram(shaders["skybox"]);
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(400.0f));

	glUniformMatrix4fv(glGetUniformLocation(shaders["skybox"], "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaders["skybox"], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaders["skybox"], "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);


	//post
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(shaders["post"]);
	VolumetricWindow::SendPostDataToShader(shaders["post"]);
	glBindVertexArray(quadVAO);

	glBindTexture(GL_TEXTURE_2D, *bufferTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	//volumetric light

	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glUseProgram(shaders["vol"]);

	glm::vec2 worldSunPos = GetSunScreenPosition(view, projection);
	glUniform2fv(glGetUniformLocation(shaders["vol"], "screenSpaceSunPosition"), 1, glm::value_ptr(worldSunPos));
	glUniform1f(glGetUniformLocation(shaders["vol"], "density"), vol.density);
	glUniform1f(glGetUniformLocation(shaders["vol"], "weight"), vol.weight);
	glUniform1f(glGetUniformLocation(shaders["vol"], "decay"), vol.decay);
	glUniform1f(glGetUniformLocation(shaders["vol"], "exposure"), vol.exposure);
	glUniform1i(glGetUniformLocation(shaders["vol"], "samples"), vol.samples);

	glUniform3fv(glGetUniformLocation(shaders["vol"], "directionalColor"), 1, glm::value_ptr(directional.diffuseColor));

	glUniform1i(glGetUniformLocation(shaders["vol"], "occTexture"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *occTexture);*/

	////run full screen
	/*glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_BLEND);*/

	if (hasGUI) {
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

void VolumetricWindow::GUIUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Light Settings");
	ImGui::Text("Exposure");
	ImGui::DragFloat("Light Exposure", (float*)&directional.lightExposure, 0.10f, 0.1f, 5.0f);
	ImGui::Text("Directional variables");
	ImGui::DragFloat3("Light Position", (float*)&directional.position);
	ImGui::DragFloat3("Light Target", (float*)&directional.target);
	ImGui::ColorEdit3("Diffuse Light Color", (float*)&directional.diffuseColor);
	ImGui::ColorEdit3("Specular Light Color", (float*)&directional.specularColor);

	ImGui::Text("Shadow variables");
	ImGui::DragFloat("Z Near", (float*)&nearPlane, 0.10f, 70.0f, 100.0f);
	ImGui::DragFloat("Z Far", (float*)&farPlane, 0.10f, 100.0f, 900.0f);
	ImGui::DragFloat("Light Angle", (float*)&lightAngle, 0.1f, 30.0f, 100.0f);

	ImGui::Text("Volumetric variables");
	ImGui::DragFloat("Density", (float*)&vol.density, 0.10, 0, 4);
	ImGui::DragFloat("Weight", (float*)&vol.weight, 0.10, 0, 4);
	ImGui::DragFloat("Decay", (float*)&vol.decay, 0.10, 0, 1);
	ImGui::DragFloat("Exposure", (float*)&vol.exposure, 0.10, 0, 4);
	ImGui::DragInt("Samples", (int*)&vol.samples, 0.10, 100, 300);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	ImGui::End();

}

void VolumetricWindow::SendPostDataToShader(GLuint shaderProgram) {

	glUniform1f(glGetUniformLocation(shaderProgram, "lightExposure"), directional.lightExposure);
	glUniform1f(glGetUniformLocation(shaderProgram, "time"), glfwGetTime());
}

void VolumetricWindow::SendLightingDataToShader(GLuint shaderProgram) {


	glUniform3fv(glGetUniformLocation(shaderProgram, "viewPosition"), 1, glm::value_ptr(camera->getCameraPosition()));
	glUniform3fv(glGetUniformLocation(shaderProgram, "lightPosition"), 1, glm::value_ptr(directional.position));
	//directional
	glUniform3fv(glGetUniformLocation(shaderProgram, "directional.position"), 1,
		glm::value_ptr(directional.position));
	glUniform3fv(glGetUniformLocation(shaderProgram, "directional.direction"), 1,
		glm::value_ptr(directional.GetDirection()));
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

	glUniform1f(glGetUniformLocation(shaderProgram, "point2.constant"), point.constant);
	glUniform1f(glGetUniformLocation(shaderProgram, "point2.linear"), point.linear);
	glUniform1f(glGetUniformLocation(shaderProgram, "point2.quadratic"), point.quadratic);

	glUniform3fv(glGetUniformLocation(shaderProgram, "point2.position"), 1, glm::value_ptr(point2.position));
	glUniform3fv(glGetUniformLocation(shaderProgram, "point2.ambient"), 1, glm::value_ptr(point2.diffuseColor));
	glUniform3fv(glGetUniformLocation(shaderProgram, "point2.diffuse"), 1, glm::value_ptr(point2.diffuseColor));
	glUniform3fv(glGetUniformLocation(shaderProgram, "point2.specular"), 1,
		glm::value_ptr(point.specularColor));

	glUniform1f(glGetUniformLocation(shaderProgram, "point.constant"), point2.constant);
	glUniform1f(glGetUniformLocation(shaderProgram, "point.linear"), point2.linear);
	glUniform1f(glGetUniformLocation(shaderProgram, "point.quadratic"), point2.quadratic);
	//spot
	glUniform3fv(glGetUniformLocation(shaderProgram, "spot.position"), 1, glm::value_ptr(spot.position));
	glUniform3fv(glGetUniformLocation(shaderProgram, "spot.direction"), 1, glm::value_ptr(spot.GetDirection()));
	glUniform1f(glGetUniformLocation(shaderProgram, "spot.cutAngle"), spot.GetCut());
	glUniform1f(glGetUniformLocation(shaderProgram, "spot.outterCutAngle"), spot.GetOutterCut());

	glUniform3fv(glGetUniformLocation(shaderProgram, "spot.ambient"), 1, glm::value_ptr(spot.diffuseColor));
	glUniform3fv(glGetUniformLocation(shaderProgram, "spot.diffuse"), 1, glm::value_ptr(spot.diffuseColor));
	glUniform3fv(glGetUniformLocation(shaderProgram, "spot.specular"), 1,
		glm::value_ptr(spot.specularColor));

	glUniform1f(glGetUniformLocation(shaderProgram, "spot.constant"), spot.constant);
	glUniform1f(glGetUniformLocation(shaderProgram, "spot.linear"), spot.linear);
	glUniform1f(glGetUniformLocation(shaderProgram, "spot.quadratic"), spot.quadratic);

	glUniform3fv(glGetUniformLocation(shaderProgram, "spot2.position"), 1, glm::value_ptr(spot2.position));
	glUniform3fv(glGetUniformLocation(shaderProgram, "spot2.direction"), 1, glm::value_ptr(spot2.GetDirection()));
	glUniform1f(glGetUniformLocation(shaderProgram, "spot2.cutAngle"), spot2.GetCut());
	glUniform1f(glGetUniformLocation(shaderProgram, "spot2.outterCutAngle"), spot2.GetOutterCut());

	glUniform3fv(glGetUniformLocation(shaderProgram, "spot2.ambient"), 1, glm::value_ptr(spot2.diffuseColor));
	glUniform3fv(glGetUniformLocation(shaderProgram, "spot2.diffuse"), 1, glm::value_ptr(spot2.diffuseColor));
	glUniform3fv(glGetUniformLocation(shaderProgram, "spot2.specular"), 1,
		glm::value_ptr(spot2.specularColor));

	glUniform1f(glGetUniformLocation(shaderProgram, "spot2.constant"), spot2.constant);
	glUniform1f(glGetUniformLocation(shaderProgram, "spot2.linear"), spot2.linear);
	glUniform1f(glGetUniformLocation(shaderProgram, "spot2.quadratic"), spot2.quadratic);

}
void VolumetricWindow::RenderSun(GLuint shader, glm::mat4& modelMatrix, glm::mat4& viewMatrix,
	glm::mat4& projectionMatrix, glm::mat4& lightMatrix) {

	glDisable(GL_DEPTH_TEST);

	VolumetricWindow::RenderModel("sphere", modelMatrix, viewMatrix, projectionMatrix, lightMatrix, shader);

	glEnable(GL_DEPTH_TEST);
}

void VolumetricWindow::RenderModel(const std::string& modelName, glm::mat4& modelMatrix, glm::mat4& viewMatrix,
	glm::mat4& projectionMatrix, glm::mat4& lightMatrix, GLuint shaderProgram) {


	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE,
		glm::value_ptr(projectionMatrix));

	models[modelName]->Draw(shaderProgram);
}

void VolumetricWindow::RenderMeshFromData(const std::string& meshName, glm::mat4& modelMatrix, GLuint shaderProgram) {

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraFront(),
		camera->getCameraUp());

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(camera->getFieldOfView()), (float)width / (float)height, 0.1f, 800.0f);


	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
		glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE,
		glm::value_ptr(projection));

	glBindVertexArray(renderables[meshName]->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void VolumetricWindow::CompileShaders() {
	ShaderManager::CompileShader(GL_VERTEX_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/BaseVS.glsl");
	ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

	ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/BaseFS.glsl");
	ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

	GLuint shaderProgram = ShaderManager::LinkShaderProgram();
	shaders["base"] = shaderProgram;
	ShaderManager::CheckShaderLink(shaders["base"]);

	ShaderManager::CompileShader(GL_VERTEX_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/EnvVS.glsl");
	ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

	ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/EnvFS.glsl");
	ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

	shaderProgram = ShaderManager::LinkShaderProgram();
	shaders["basescene"] = shaderProgram;
	ShaderManager::CheckShaderLink(shaders["basescene"]);

	ShaderManager::CompileShader(GL_VERTEX_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/PostVS.glsl");
	ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

	ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/PostFS.glsl");
	ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

	shaderProgram = ShaderManager::LinkShaderProgram();
	shaders["post"] = shaderProgram;
	ShaderManager::CheckShaderLink(shaders["post"]);

	ShaderManager::CompileShader(GL_VERTEX_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/SkyVS.glsl");
	ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

	ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/SkyFS.glsl");
	ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

	shaderProgram = ShaderManager::LinkShaderProgram();
	shaders["skybox"] = shaderProgram;
	ShaderManager::CheckShaderLink(shaders["skybox"]);

	ShaderManager::CompileShader(GL_VERTEX_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/DepthVS.glsl");
	ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

	ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/DepthFS.glsl");
	ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

	shaderProgram = ShaderManager::LinkShaderProgram();
	shaders["depth"] = shaderProgram;
	ShaderManager::CheckShaderLink(shaders["depth"]);

	ShaderManager::CompileShader(GL_VERTEX_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/LighDepthVS.glsl");
	ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

	ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/LightDepthFS.glsl");
	ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

	shaderProgram = ShaderManager::LinkShaderProgram();
	shaders["lightdepth"] = shaderProgram;
	ShaderManager::CheckShaderLink(shaders["lightdepth"]);

	ShaderManager::CompileShader(GL_VERTEX_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/VolumetricVS.glsl");
	ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

	ShaderManager::CompileShader(GL_FRAGMENT_SHADER, "C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/shaders/VolumetricFS.glsl");
	ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

	shaderProgram = ShaderManager::LinkShaderProgram();
	shaders["vol"] = shaderProgram;
	ShaderManager::CheckShaderLink(shaders["vol"]);
}

void VolumetricWindow::OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

}

void VolumetricWindow::OnCursorPositionChange(GLFWwindow* window, double xPosition, double yPosition) {
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

void VolumetricWindow::OnScrollChange(GLFWwindow* window, double xOffset, double yOffset) {
	camera->setFieldOfView(camera->getFieldOfView() - yOffset);
	float foV = camera->getFieldOfView();
	if (foV < 1.0f) camera->setFieldOfView(1.0f);
	if (foV > 45.0f) camera->setFieldOfView(45.0f);
}

void VolumetricWindow::OnInputUpdate() {
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

void VolumetricWindow::OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {

	//glViewport(0, 0, width, height);
}


VolumetricWindow::~VolumetricWindow() {
	for (auto const& renderable : renderables) {
		delete renderable.second;
	}
	renderables.clear();
	for (auto const& model : models) {
		delete model.second;
	}
	models.clear();
	delete camera;
}

GLFWwindow* VolumetricWindow::GetWindowContext() {
	return window;
}

int VolumetricWindow::GetWindowHeight() {
	return height;
}

int VolumetricWindow::GetWindowWidth() {
	return width;
}

unsigned int VolumetricWindow::LoadCubeMap(const std::vector<std::string>& faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
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

void VolumetricWindow::InitCubeMap() {
	float cubeVertices[] = {

			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
	};

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	std::vector<std::string> cubeMapFaces{
			"C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/resources/skybox/pinkright.png",
			"C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/resources/skybox/pinkleft.png",
			"C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/resources/skybox/pinkup.png",
			"C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/resources/skybox/pinkbottom.png",
			"C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/resources/skybox/pinkfront.png",
			"C:/Users/Laprinia/source/repos/hyzu-visual/hyzu-visual/resources/skybox/pinkback.png",
	};
	cubemapTexture = VolumetricWindow::LoadCubeMap(cubeMapFaces);
}

glm::vec2 VolumetricWindow::GetSunScreenPosition(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

	glm::vec4 clipSpacePosition = projectionMatrix * (viewMatrix * glm::vec4(directional.position, 1.0));

	glm::vec3 ndcSpaceCoord = glm::vec3(clipSpacePosition.x, clipSpacePosition.y, clipSpacePosition.z) / (clipSpacePosition.w);

	glm::vec2 windowSpacePos = ((glm::vec2(ndcSpaceCoord) + glm::vec2(1.0f)) / glm::vec2(2.0f));

	return windowSpacePos;
}