//
// Created by Lavinia
//

#include "GUIManager.h"


GUIManager::GUIManager(GLFWwindow* windowContext) {
	GUIManager::windowContext = windowContext;
};

GLFWwindow* GUIManager::windowContext = nullptr;

GUIManager& GUIManager::CreateContext(GLFWwindow* windowContext) {
	static GUIManager instance(windowContext);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplGlfw_InitForOpenGL(windowContext, true);
	ImGui_ImplOpenGL3_Init("#version 460");
	ImGui::StyleColorsDark();
	return instance;
}

void GUIManager::DeleteContext()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

void GUIManager::DrawData()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void GUIManager::DrawDirectionalWindow(DirectionalLight* directional, VolLight* vol, float* nearPlane, float* farPlane, float* lightAngle)
{
	ImGui::Begin("Directional Settings");
	ImGui::Text("Exposure");
	ImGui::DragFloat("Light Exposure", &directional->lightExposure, 0.10f, 0.1f, 5.0f);
	ImGui::Text("Directional variables");
	ImGui::DragFloat3("Light Position", (float*)&directional->position);
	ImGui::DragFloat3("Light Target", (float*)&directional->target);
	ImGui::ColorEdit3("Diffuse Light Color", (float*)&directional->diffuseColor);
	ImGui::ColorEdit3("Specular Light Color", (float*)&directional->specularColor);

	ImGui::Text("Shadow variables");
	ImGui::DragFloat("Z Near", (float*)nearPlane, 0.10f, 70.0f, 100.0f);
	ImGui::DragFloat("Z Far", (float*)farPlane, 0.10f, 100.0f, 900.0f);
	ImGui::DragFloat("Light Angle", (float*)lightAngle, 0.1f, 30.0f, 100.0f);

	ImGui::Text("Volumetric variables");
	ImGui::DragFloat("Density", (float*)&vol->density, 0.10, 0, 4);
	ImGui::DragFloat("Weight", (float*)&vol->weight, 0.10, 0, 4);
	ImGui::DragFloat("Decay", (float*)&vol->decay, 0.10, 0, 1);
	ImGui::DragFloat("Exposure", (float*)&vol->exposure, 0.10, 0, 4);
	ImGui::DragInt("Samples", (int*)&vol->samples, 0.10, 100, 300);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

const int GUIManager::DrawSceneSelectionWindow(bool *hasBaseScene) {

	ImGui::Begin("Scene Selection Settings");
	static int selectedItem = 0;
	const char* scenes[] = {"Pool","Bath"};
	ImGui::Combo("Select Scene",&selectedItem,scenes, IM_ARRAYSIZE(scenes));
	
	ImGui::End();
	return selectedItem;
}


