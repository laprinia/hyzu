//
// Created by Lavinia
//

#include "GUIManager.h"
#include <iostream>


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
	ImGui::DragFloat("Decay", (float*)&vol->decay, 0.10, 0, 20);
	ImGui::DragFloat("Exposure", (float*)&vol->exposure, 0.10, 0, 4);
	ImGui::DragInt("Samples", (int*)&vol->samples, 0.10, 100, 300);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

const int GUIManager::DrawSceneSelectionWindow(bool* hasDebugLights, DirectionalLight* directional, PointLight* point, PointLight* point2, SpotLight* spot, SpotLight* spot2) {

	ImGui::Begin("Scene Selection Settings");
	ImGui::Checkbox("Debug Lights", hasDebugLights);
	static int selectedItem = 0;
	const char* scenes[] = { "Pool","Bath" };
	if (ImGui::Combo("Select Scene", &selectedItem, scenes, IM_ARRAYSIZE(scenes))) {
		if (selectedItem == 1) {
			directional->lightExposure = 0.4;
			directional->position = glm::vec3(-100, 12, 64);
			directional->diffuseColor = glm::vec3(1.0f, 0.4784f, 0.42745f);
			directional->specularColor = glm::vec3(1.0f, 0.96078f, 0.42745f);

			spot->position = glm::vec3(12.0f, 7.0f, 27.0f);
			spot->target = glm::vec3(12.0f, -81.0f, 20.0f);
			spot->outterCutAngle = 23.0f;
			spot->cutAngle = 19.5f;
			spot->diffuseColor = glm::vec3(0.2235f, 0.95294f, 0.172549f);
			spot->specularColor = glm::vec3(0.8235f, 1.0f, 0.78039f);

			spot2->position = glm::vec3(-6.0f, 7.0f, 27.0f);
			spot2->target = glm::vec3(-6.0f, -81.0f, 20.0f);
			spot2->outterCutAngle = 23.0f;
			spot2->cutAngle = 19.5f;
			spot2->diffuseColor = glm::vec3(0.2235f, 0.95294f, 0.172549f);
			spot2->specularColor = glm::vec3(0.8235f, 1.0f, 0.78039f);

			point->position = glm::vec3(-10.0f, -3.0f, 40.0f);
			point->diffuseColor = glm::vec3(0.317647f, 0.898f, 0.937f);
			point->specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

			point2->position = glm::vec3(16.0, 0.0f, 47.0f);
			point2->diffuseColor = glm::vec3(0.317647f, 0.898f, 0.937f);
			point2->specularColor = glm::vec3(1.0f, 1.0f, 1.0f);


		}
		else {
			directional->lightExposure = 0.3;
			directional->position = glm::vec3(-48, 31, -83);
			directional->diffuseColor = glm::vec3(0.4275f, 0.4157f, 0.82745f);
			directional->specularColor = glm::vec3(0.8039f, 0.8078f, 1.0f);

			spot->position = glm::vec3(23, 30, 41);
			spot->target = glm::vec3(24, 14, 4);
			spot->outterCutAngle = 14;
			spot->cutAngle = 13.5f;
			spot->diffuseColor = glm::vec3(1.0f, 0.6196078f, 0.32941f);
			spot->specularColor = glm::vec3(1.0f, 0.780392f, 0.643137f);

			spot2->position = glm::vec3(26, 30, -27);
			spot2->target = glm::vec3(-11, 2, 15);
			spot2->outterCutAngle = 14;
			spot2->cutAngle = 13.5f;
			spot2->diffuseColor = glm::vec3(1.0f, 0.6196078f, 0.32941f);
			spot2->specularColor = glm::vec3(1.0f, 0.780392f, 0.643137f);

			point->position = glm::vec3(1.0f, 16.0f, -38.0f);
			point->diffuseColor = glm::vec3(0.650980f, 0.1254902f, 0.8980392f);
			point->specularColor = glm::vec3(1.0f, 0.16470f, 0.4666667f);


			point2->position = glm::vec3(1.0f, 16.0f, 38.0f);
			point2->diffuseColor = glm::vec3(0.650980f, 0.1254902f, 0.8980392f);
			point2->specularColor = glm::vec3(1.0f, 0.16470f, 0.4666667f);
		}
	}

	ImGui::End();
	return selectedItem;
}

void GUIManager::DrawSceneData(Scene* scene) {
	ImGui::Begin("Current Scene Data");
	std::string currentSkybox = "Current Skybox ID:" + scene->skybox;
	ImGui::Text(currentSkybox.c_str());
	ImGui::Text("Scene Items");
	int i = 0;
	for (ImportedModel* model : scene->models) {

		std::string modelName = model->name + (i == 0 ? "" : "[ " + std::to_string(i + 1) + " ]");
		if (ImGui::TreeNode(modelName.c_str())) {
			ImGui::Checkbox("is visible", &model->isVisible);
			int j = 0;
			for (ImportedModel& sub : model->models) {
				std::string subName = sub.name + (j == 0 ? "" : "[ " + std::to_string(j + 1) + " ]");
				if (ImGui::TreeNode(subName.c_str())) {
					bool noRoot = false;
					ImGui::Checkbox("Visible", model->isVisible ? &sub.isVisible : &noRoot);
					ImGui::DragFloat3("Position", &sub.translate[0]);
					ImGui::DragFloat("Scale", &sub.scale);
					ImGui::TreePop();
					ImGui::Separator();
					j++;
				}
			}
			ImGui::TreePop();
			ImGui::Separator();
			i++;
		}
	}

	ImGui::End();
}

void GUIManager::DrawLightData(PointLight* point1, PointLight* point2, SpotLight* spot1, SpotLight* spot2) {
	ImGui::Begin("Current Light Data");

	if (ImGui::CollapsingHeader("Point Light 1")) {
		ImGui::DragFloat3("Point Light Position", (float*)&point1->position);
		ImGui::ColorEdit3("Point Diffuse Light Color", (float*)&point1->diffuseColor);
		ImGui::ColorEdit3("Point Specular Light Color", (float*)&point1->specularColor);
		ImGui::DragFloat("Point Constant", &point1->constant);
		ImGui::DragFloat("Point Linear", &point1->linear);
		ImGui::DragFloat("Point Quadratic", &point1->quadratic);

	}
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Point Light 2")) {
		ImGui::DragFloat3("Point Light Position 2", (float*)&point2->position);
		ImGui::ColorEdit3("Point Diffuse Light Color 2", (float*)&point2->diffuseColor);
		ImGui::ColorEdit3("Point Specular Light Color 2", (float*)&point2->specularColor);
		ImGui::DragFloat("Point Constant 2", &point2->constant);
		ImGui::DragFloat("Point Linear 2", &point2->linear);
		ImGui::DragFloat("Point Quadratic 2", &point2->quadratic);
	}
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Spot Light 1")) {
		ImGui::DragFloat3("Spot Light Position", (float*)&spot1->position);
		ImGui::DragFloat3("Spot Light Target", (float*)&spot1->target);
		ImGui::ColorEdit3("Spot Diffuse Light Color", (float*)&spot1->diffuseColor);
		ImGui::ColorEdit3("Spot Specular Light Color", (float*)&spot1->specularColor);
		ImGui::DragFloat("Spot Outter Cut", &spot1->outterCutAngle);
		ImGui::DragFloat("Spot Cut", &spot1->cutAngle);

	}
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Spot Light 2")) {
		ImGui::DragFloat3("Spot Light Position 2", (float*)&spot2->position);
		ImGui::DragFloat3("Spot Light Target 2", (float*)&spot2->target);
		ImGui::ColorEdit3("Spot Diffuse Light Color 2", (float*)&spot2->diffuseColor);
		ImGui::ColorEdit3("Spot Specular Light Color 2", (float*)&spot2->specularColor);
		ImGui::DragFloat("Spot Outter Cut 2", &spot2->outterCutAngle);
		ImGui::DragFloat("Spot Cut 2", &spot2->cutAngle);
	}
	ImGui::End();
}

void GUIManager::DrawOccTexture(unsigned int occTexture)
{
ImGui::Begin("Occlusion Texture");
ImVec2 winSize = ImGui::GetWindowSize();
ImGui::Image((void*)occTexture, winSize, ImVec2(0, 1),
	ImVec2(1, 0));
ImGui::End();
}


