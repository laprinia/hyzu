#pragma once
#include <iostream>
#include <json.h>
#include <fstream>
#include <string>
#include <vector>
#include "render/ImportedModel.h"
class Scene
{
public:
	Scene(const std::string& path);
	std::string skybox;
	std::vector<ImportedModel*> models;

};

