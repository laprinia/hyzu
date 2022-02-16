#pragma once
#include <string>
#include <glm.hpp>
class ImportedModel
{
public:
	std::string name;
	float scale;
	float rotate;
	glm::vec3 rotateAxis;
	glm::vec3 translate;
	float hasBlend;
	ImportedModel(const std::string& name, float scale, float rotate, glm::vec3 rotateAxis, glm::vec3 translate, float hasBlend);
	void ToString();
};

