#pragma once
#include "render/SimpleMesh.h"
#include <stb_image_aug.h>
class Skybox
{
private:
	SimpleMesh* simpleMesh;

	unsigned int cubeVAO;
	unsigned int cubeVBO;
	unsigned int cubemapTexture;

public:
	Skybox(const std::string& texturePath);
	unsigned int LoadCubeMap(const std::vector<std::string>& faces);
	void Draw();
};

