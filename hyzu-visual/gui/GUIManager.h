//
// Created by Lavinia
//

#ifndef HYZU_GUIMANAGER_H
#define HYZU_GUIMANAGER_H
#include <GLFW/glfw3.h>
#include "../lighting/LightSources.h"
#include "../includes/imgui/imgui.h"
#include "../includes/imgui/imgui_impl_glfw.h"
#include "../includes/imgui/imgui_impl_opengl3.h"
#include <vector>
#include <string>
#include "../Scene.h"


class GUIManager {
private:
	static GLFWwindow* windowContext;
	GUIManager() = default;
	GUIManager(GLFWwindow* windowContext);

public:

	static GUIManager& CreateContext(GLFWwindow* windowContext);
	static void DrawLightData( PointLight* point1, PointLight* point2, SpotLight* spot1, SpotLight* spot2);
	static void DrawData();
	static void DeleteContext();
	static void DrawSceneData(Scene* scene);
	static void DrawDirectionalWindow(DirectionalLight* directional, VolLight* vol, float* nearPlane, float* farPlane, float* lightAngle);
	static const int DrawSceneSelectionWindow(bool* hasDebugLights, DirectionalLight* dir, PointLight* point1, PointLight* point2, SpotLight* spot1, SpotLight* spot2);
	static void DrawOccTexture(unsigned int occTexture);
};
#endif
