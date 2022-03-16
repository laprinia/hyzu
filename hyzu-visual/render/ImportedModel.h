#pragma once
#include <string>
#include <glm.hpp>
#include <vector>
class ImportedModel
{
public:
	std::string name;
	float scale;
	float rotate;
	glm::vec3 rotateAxis;
	glm::vec3 translate;
	float hasBlend;
	bool isVisible;
	std::vector<ImportedModel> models;

	ImportedModel(const std::string& name, float scale, float rotate, glm::vec3 rotateAxis, glm::vec3 translate, float hasBlend, std::vector<ImportedModel> models, bool isVisible);
	void ToString();
};

