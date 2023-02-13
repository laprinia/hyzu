//
// Created by Lavinia
//

#include "SampleWindow.h"
#include "json.h"

bool SampleWindow::hasGUI = false;
Camera* SampleWindow::camera = nullptr;
float SampleWindow::cameraSpeed = 20.0f;
float SampleWindow::inputDeltaTime = 0.0f;
float SampleWindow::inputLastFrame = 0.0f;
float SampleWindow::updateDeltaTime = 0.0f;
float SampleWindow::updateLastFrame = 0.0f;
glm::vec3 SampleWindow::orbitOrigin = glm::vec3(2.867020f, 11.502365f, 1.150090f);
bool SampleWindow::firstMouseMove = true;
double SampleWindow::lastMouseX = 0.0f;
double SampleWindow::lastMouseY = 0.0f;
double SampleWindow::yaw = -90.0f;
double SampleWindow::pitch = 0.0f;
float SampleWindow::mouseSensitivity = 0.1f;
int SampleWindow::width = 0;
int SampleWindow::height = 0;


SampleWindow::SampleWindow(int width, int height, const std::string& title) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 64);
	this->width = width;
	this->height = height;
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create the GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	GLFWimage icon;
	icon.pixels = SOIL_load_image(".\\..\\hyzu-visual\\resources\\icon_dark.png", &icon.width, &icon.height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(window, 1, &icon);
	SOIL_free_image_data(icon.pixels);
	glewExperimental = GL_TRUE;
	InputManager::GetInstance(window);
	InputManager::SetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	InputManager::SetWindowKeyCallback(window, OnKeyPress);
	InputManager::SetWindowCursorPositionCallback(window, OnCursorPositionChange);
	InputManager::SetWindowScrollCallback(window, OnScrollChange);
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
	}
	glViewport(0, 0, width, height);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	SampleWindow::AddShaders();
	SampleWindow::Init();

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

		SampleWindow::Update();
	}

	GUIManager::DeleteContext();

	//TODO delete vaos vbos

	glfwTerminate();
}

void SampleWindow::Init() {


	GUIManager::CreateContext(window);


	camera = new Camera(glm::vec3(16.319027f, 9.407349f, 39.400589f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//models 
	std::vector<SVertex> screenQuadVertices = {

			SVertex{glm::vec3(-1.0f,-1.0f,0.0f),glm::vec2(0.0f,0.0f)},
			SVertex{glm::vec3(-1.0f,1.0f,0.0f),glm::vec2(0.0f,1.0f)},
			SVertex{glm::vec3(1.0f,1.0f,0.0f),glm::vec2(1.0f,1.0f)},
			SVertex{glm::vec3(1.0f,-1.0f,0.0f),glm::vec2(1.0f,0.0f)}
	};

	std::vector<GLuint> screenQuadIndices = { 0, 1, 2,
		0, 2, 3 };
	SimpleMesh* simpleMesh = new SimpleMesh(screenQuadVertices, screenQuadIndices);
	simpleMeshes["quad"] = simpleMesh;

	Model* model1 = new Model(".\\..\\hyzu-visual\\resources/scenes/pool/base.obj");
	models["basescene"] = model1;
	Model* model2 = new Model(".\\..\\hyzu-visual\\resources/scenes/pool/water/poolwater.obj");
	models["poolwater"] = model2;

	Model* model3 = new Model(".\\..\\hyzu-visual\\resources/scenes/pool/sphere.obj");
	models["sphere"] = model3;

	Model* model4 = new Model(".\\..\\hyzu-visual\\resources/scenes/room/4/bath.obj");
	models["bath"] = model4;

	Model* model5 = new Model(".\\..\\hyzu-visual\\resources/scenes/soap/scene.gltf");
	models["soap"] = model5;


	Skybox* skybox = new Skybox(".\\..\\hyzu-visual\\resources\\skybox\\pink");
	skyboxes["pink"] = skybox;

	Skybox* skybox2 = new Skybox(".\\..\\hyzu-visual\\resources\\skybox\\ocean");
	skyboxes["ocean"] = skybox2;

	Skybox* skybox3 = new Skybox(".\\..\\hyzu-visual\\resources\\skybox\\blue");
	skyboxes["blue"] = skybox3;


	//lights 
	point2.position = glm::vec3(1.0f, 16.0f, 38.0f);

	spot2.position = glm::vec3(26, 30, -27);
	spot2.target = glm::vec3(-11, 2, 15);

	//scenes

	Scene* scene = new Scene(".\\..\\hyzu-visual\\files/scenes/Pool.json");
	loadedScenes["Pool"] = scene;

	Scene* scene2 = new Scene(".\\..\\hyzu-visual\\files/scenes/Bath.json");
	loadedScenes["Bath"] = scene2;
}

void SampleWindow::RenderScene(GLuint  shader, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, bool isDepthPass,
	glm::mat4& lightMatrix) {

	Scene* currentlySelected = loadedScenes[scenesIDs[selectedScene]];

	//render light locations
	glm::mat4 model = glm::mat4(1.0f);

	if (hasDebugLights) {
		model = glm::translate(model, point.position);
		model = glm::scale(model, glm::vec3(0.4f));
		RenderModel("sphere", model, viewMatrix, projectionMatrix, lightMatrix, isDepthPass ? shaders["depth"] : shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, point2.position);
		model = glm::scale(model, glm::vec3(0.4f));
		RenderModel("sphere", model, viewMatrix, projectionMatrix, lightMatrix, isDepthPass ? shaders["depth"] : shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, spot.position);
		model = glm::scale(model, glm::vec3(0.4f));
		RenderModel("sphere", model, viewMatrix, projectionMatrix, lightMatrix, isDepthPass ? shaders["depth"] : shader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, spot2.position);
		model = glm::scale(model, glm::vec3(0.4f));
		RenderModel("sphere", model, viewMatrix, projectionMatrix, lightMatrix, isDepthPass ? shaders["depth"] : shader);

	}

	for (ImportedModel* imported : currentlySelected->models) {

		if (imported->isVisible) {
			model = glm::mat4(1.0f);

			model = glm::scale(model, glm::vec3(imported->scale));
			model = glm::rotate(model, glm::radians(imported->rotate), imported->rotateAxis);

			model = glm::translate(model, imported->translate);

			SampleWindow::SendLightingDataToShader(isDepthPass ? shaders["depth"] : shader);
			if (imported->hasBlend) {
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
			}
			SampleWindow::RenderModel(imported->name, model, viewMatrix, projectionMatrix, lightMatrix,
				isDepthPass ? shaders["depth"] : shader);
			if (imported->hasBlend) glDisable(GL_BLEND);

			for (ImportedModel sub : imported->models) {
				if (sub.isVisible) {
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::scale(model, glm::vec3(sub.scale));
					model = glm::rotate(model, glm::radians(sub.rotate), sub.rotateAxis);
					model = glm::translate(model, sub.translate);

					if (sub.hasBlend) {
						glEnable(GL_BLEND);
						glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
					}
					SampleWindow::RenderModel(sub.name, model, viewMatrix, projectionMatrix, lightMatrix,
						isDepthPass ? shaders["depth"] : shader);
					if (sub.hasBlend) glDisable(GL_BLEND);
				}
			}
		}

	}

}

auto SampleWindow::RotateAround(float rad, const glm::vec3& point, const glm::vec3& axis) {
	auto t1 = glm::translate(glm::mat4(1), -point);
	auto r = glm::rotate(glm::mat4(1), rad, axis);
	auto t2 = glm::translate(glm::mat4(1), point);
	return t2 * r * t1;
}

void SampleWindow::Update() {


	if (!hasGUI) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	glfwPollEvents();
	OnInputUpdate();
	GUIUpdate();
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
	SampleWindow::RenderSun(shaders["base"], model, view, projection, empty);

	glUniform1i(glGetUniformLocation(shaders["base"], "isTransparent"), 0);
	glUniform3fv(glGetUniformLocation(shaders["base"], "solidColor"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
	SampleWindow::RenderScene(shaders["base"], view, projection, false, empty);


	//depth pass
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shaders["depth"]);
	glViewport(0, 0, depth_width_height, depth_width_height);
	glBindFramebuffer(GL_FRAMEBUFFER, *depthID);
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 bogus = glm::mat4(0);
	SampleWindow::RenderScene(shaders["basescene"], bogus, bogus, true, lightSpaceMatrix);


	//normal pass
	glBindFramebuffer(GL_FRAMEBUFFER, *fbID);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//light + rotation
	glUseProgram(shaders["base"]);
	model = glm::mat4(1.0f);
	float updateCurrentTime = glfwGetTime();
	updateDeltaTime = updateCurrentTime - updateLastFrame;
	updateLastFrame = updateCurrentTime;
	float actualSpeed = 0.03f * updateDeltaTime;
	std::cout << glm::to_string(camera->getCameraPosition()) << std::endl;
	auto modelResult = RotateAround(-actualSpeed, orbitOrigin, glm::vec3(0, 1, 0)) *
		glm::translate(glm::mat4(1), directional.position) *
		glm::scale(model, glm::vec3(20.0f));

	glm::vec3 currentPosition = glm::vec3(modelResult[3][0], modelResult[3][1], modelResult[3][2]);
	directional.position = currentPosition;
	glUniform1i(glGetUniformLocation(shaders["base"], "isTransparent"), 1);
	glUniform3fv(glGetUniformLocation(shaders["base"], "solidColor"), 1, glm::value_ptr(directional.diffuseColor));
	SampleWindow::RenderSun(shaders["base"], modelResult, view, projection, lightSpaceMatrix);

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

	SampleWindow::RenderScene(shaders["basescene"], view, projection, false, lightSpaceMatrix);

	//skybox
	glDepthFunc(GL_LEQUAL);
	glUseProgram(shaders["skybox"]);
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(400.0f));

	glUniformMatrix4fv(glGetUniformLocation(shaders["skybox"], "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaders["skybox"], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shaders["skybox"], "model"), 1, GL_FALSE, glm::value_ptr(model));

	Scene* currentlySelected = loadedScenes[scenesIDs[selectedScene]];
	skyboxes[currentlySelected->skybox]->Draw();
	glDepthFunc(GL_LESS);


	//post
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(shaders["post"]);
	SampleWindow::SendPostDataToShader(shaders["post"]);

	glBindTexture(GL_TEXTURE_2D, *bufferTexture);
	simpleMeshes["quad"]->Draw();


	//volumetric light

	glEnable(GL_BLEND);
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
	glBindTexture(GL_TEXTURE_2D, *occTexture);

	////run full screen
	simpleMeshes["quad"]->Draw();
	glDisable(GL_BLEND);


	GUIManager::DrawData();
	glfwSwapBuffers(window);

}

void SampleWindow::GUIUpdate() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	if (hasGUI)
	{
		GUIManager::DrawDirectionalWindow(&directional, &vol, &nearPlane, &farPlane, &lightAngle);
		selectedScene = GUIManager::DrawSceneSelectionWindow(&hasDebugLights, &directional, &point, &point2, &spot, &spot2);
		GUIManager::DrawSceneData(loadedScenes[scenesIDs[selectedScene]]);
		GUIManager::DrawLightData(&point, &point2, &spot, &spot2);
		GUIManager::DrawOccTexture(*occTexture);
	}

	ImGui::EndFrame();

}

void SampleWindow::SendPostDataToShader(GLuint shaderProgram) {

	glUniform1f(glGetUniformLocation(shaderProgram, "lightExposure"), directional.lightExposure);
	glUniform1f(glGetUniformLocation(shaderProgram, "time"), glfwGetTime());
}

void SampleWindow::ChangeLightingData()
{

}

void SampleWindow::SendLightingDataToShader(GLuint shaderProgram) {


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
void SampleWindow::RenderSun(GLuint shader, glm::mat4& modelMatrix, glm::mat4& viewMatrix,
	glm::mat4& projectionMatrix, glm::mat4& lightMatrix) {

	glDisable(GL_DEPTH_TEST);

	SampleWindow::RenderModel("sphere", modelMatrix, viewMatrix, projectionMatrix, lightMatrix, shader);

	glEnable(GL_DEPTH_TEST);
}

void SampleWindow::RenderModel(const std::string& modelName, glm::mat4& modelMatrix, glm::mat4& viewMatrix,
	glm::mat4& projectionMatrix, glm::mat4& lightMatrix, GLuint shaderProgram) {


	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE,
		glm::value_ptr(projectionMatrix));

	models[modelName]->Draw(shaderProgram);
}

void SampleWindow::RenderMeshFromData(const std::string& meshName, glm::mat4& modelMatrix, GLuint shaderProgram) {

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


void SampleWindow::AddShaders() {

	shaders["base"] = ShaderManager::AddShader("Base");
	shaders["basescene"] = ShaderManager::AddShader("Env");
	shaders["post"] = ShaderManager::AddShader("Post");
	shaders["skybox"] = ShaderManager::AddShader("Sky");
	shaders["depth"] = ShaderManager::AddShader("Depth");
	shaders["lightdepth"] = ShaderManager::AddShader("LightDepth");
	shaders["vol"] = ShaderManager::AddShader("Volumetric");

}

void SampleWindow::OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		hasGUI ^= true;
		if (hasGUI == false) {

			glfwSetCursorPos(window, width / 2, height / 2);
		}
	}
}

void SampleWindow::OnCursorPositionChange(GLFWwindow* window, double xPosition, double yPosition) {
	if (!hasGUI) {
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
}

void SampleWindow::OnScrollChange(GLFWwindow* window, double xOffset, double yOffset) {

	camera->setFieldOfView(camera->getFieldOfView() - yOffset);
	float foV = camera->getFieldOfView();
	if (foV < 1.0f) camera->setFieldOfView(1.0f);
	if (foV > 45.0f) camera->setFieldOfView(45.0f);
}

void SampleWindow::OnInputUpdate() {
	float inputCurrentTime = glfwGetTime();
	inputDeltaTime = inputCurrentTime - inputLastFrame;
	inputLastFrame = inputCurrentTime;
	float actualSpeed = cameraSpeed * inputDeltaTime;

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

void SampleWindow::OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height);
}


SampleWindow::~SampleWindow() {
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

GLFWwindow* SampleWindow::GetWindowContext() {
	return window;
}

int SampleWindow::GetWindowHeight() {
	return height;
}

int SampleWindow::GetWindowWidth() {
	return width;
}

glm::vec2 SampleWindow::GetSunScreenPosition(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {

	glm::vec4 clipSpacePosition = projectionMatrix * (viewMatrix * glm::vec4(directional.position, 1.0));

	glm::vec3 ndcSpaceCoord = glm::vec3(clipSpacePosition.x, clipSpacePosition.y, clipSpacePosition.z) / (clipSpacePosition.w);

	glm::vec2 windowSpacePos = ((glm::vec2(ndcSpaceCoord) + glm::vec2(1.0f)) / glm::vec2(2.0f));

	return windowSpacePos;
}