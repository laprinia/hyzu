#include "ImportedModel.h"
#include <iostream>

ImportedModel::ImportedModel(const std::string& name, float scale, float rotate, glm::vec3 rotateAxis, glm::vec3 translate, float hasBlend, std::vector<ImportedModel> importedModels, bool isVisible)
{
	this->name = name;
	this->scale = scale;
	this->rotate = rotate;
	this->rotateAxis = rotateAxis;
	this->translate = translate;
	this->hasBlend = hasBlend;
	this->models = importedModels;
	this->isVisible = isVisible;
}

void ImportedModel::ToString()
{
	std::cout << this->name << "-" << this->scale << "-" << this->rotate << "-" << this->rotateAxis[0] << "-" << this->translate[0] << std::endl;
}
