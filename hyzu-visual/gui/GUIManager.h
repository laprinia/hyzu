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


class GUIManager {
private:
	static GLFWwindow* windowContext;
	GUIManager() = default;
	GUIManager(GLFWwindow* windowContext);

public:

	static GUIManager& CreateContext(GLFWwindow* windowContext);
	static void DeleteContext();
	static void DrawData();
	static void DrawDirectionalWindow(DirectionalLight directional, VolLight vol, float nearPlane, float farPlane, float lightAngle);
};
#endif
