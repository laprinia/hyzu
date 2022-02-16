
#pragma once
#ifndef HYZU_SAMPWINDOW_H
#define HYZU_SAMPWINDOW_H
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "../input/InputManager.h"
#include "../shaders/ShaderManager.h"
#include "../gui/GUIManager.h"
#include "../render/RenderableWithVertexColor.h"
#include "../render/SimpleMesh.h"
#include "../camera/Camera.h"
#include "../render/Model.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "../includes/imgui/imgui.h"
#include "../includes/imgui/imgui_impl_glfw.h"
#include <stb_image_aug.h>
#include "../includes/imgui/imgui_impl_opengl3.h"
#include "../Skybox.h"
#include "../Scene.h"

class SampleWindow {
private:
	static Camera* camera;
	GLFWwindow* window;
	bool hasGUI = false;
	bool isBaseScene = true;
	unsigned int* fbID;
	unsigned int* depthID;
	unsigned int* occID;
	unsigned int* bufferTexture;
	unsigned int* depthTexture;
	unsigned int* occTexture;

	const unsigned int depth_width_height = 1024;
	float nearPlane = 40.0f, farPlane = 800.0f;
	float lightAngle = 35.9f;
	int width, height;
	static bool firstMouseMove;
	static double lastMouseX, lastMouseY;
	static double yaw, pitch;
	static float mouseSensitivity;
	static float cameraSpeed;
	static float deltaTime;
	static float lastFrame;
	std::unordered_map<std::string, RenderableObject*> renderables;
	std::unordered_map<std::string, SimpleMesh*> simpleMeshes;
	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, Skybox*> skyboxes;
	std::unordered_map<std::string, Scene*> loadedScenes;
	std::unordered_map<std::string, GLuint> shaders;
	int selectedScene = 0;
	std::vector<std::string> scenesIDs = { "Pool","Bath" };

	DirectionalLight directional;
	PointLight point, point2;
	SpotLight spot, spot2;
	VolLight vol;

	void AddShaders();

	static void OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mode);

	static void OnCursorPositionChange(GLFWwindow* window, double xPosition, double yPosition);

	static void OnScrollChange(GLFWwindow* window, double xOffset, double yOffset);

	static void OnFramebufferSizeChange(GLFWwindow* window, int width, int height);


public:
	SampleWindow(int width, int height, const std::string& title);

	~SampleWindow();

	void Update();

	void Init();

	GLFWwindow* GetWindowContext();

	int GetWindowHeight();

	int GetWindowWidth();

	void RenderMeshFromData(const std::string& meshName, glm::mat4& modelMatrix, GLuint shaderProgram);

	void RenderModel(const std::string& modelName, glm::mat4& modelMatrix, glm::mat4& viewMatrix,
		glm::mat4& projectionMatrix, glm::mat4& lightMatrix, GLuint shaderProgram);


	void SendLightingDataToShader(GLuint shaderProgram);

	void SendPostDataToShader(GLuint shaderProgram);

	void OnInputUpdate();

	void GUIUpdate();

	void RenderScene(GLuint  shader, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, bool isDepthPass, glm::mat4& lightMatrix);

	void RenderSun(GLuint shader, glm::mat4& modelMatrix, glm::mat4& viewMatrix,
		glm::mat4& projectionMatrix, glm::mat4& lightMatrix);

	glm::vec2 GetSunScreenPosition(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
};

#endif //HYZU_SAMPWINDOW_H
