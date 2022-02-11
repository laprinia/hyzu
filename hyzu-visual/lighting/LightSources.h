#include <glm.hpp>

struct DirectionalLight {
	glm::vec3 diffuseColor = glm::vec3(0.4275f, 0.4157f, 0.82745f);
	glm::vec3 specularColor = glm::vec3(0.8039f, 0.8078f, 1.0f);
	glm::vec3 position = glm::vec3(-48, 31, -83);
	glm::vec3 target = glm::vec3(0, 0, 0);

	glm::vec3 GetDirection() const {
		return -(glm::normalize(position - target));
	}

	float lightExposure = 0.3f;
};


struct PointLight {
	glm::vec3 diffuseColor = glm::vec3(0.650980f, 0.1254902f, 0.8980392f);
	glm::vec3 specularColor = glm::vec3(1.0f, 0.16470f, 0.4666667f);
	glm::vec3 position = glm::vec3(1.0f, 16.0f, -38.0f);

	float constant = 0.20f;
	float linear = 0.030f;
	float quadratic = 0.001f;
};

struct SpotLight {
	glm::vec3 position = glm::vec3(23, 30, 41);
	glm::vec3 target = glm::vec3(24, 14, 4);
	glm::vec3 GetDirection() const {
		return (glm::normalize(position - target));
	}
	float outterCutAngle = 14;
	float GetOutterCut() {
		return glm::cos(glm::radians(outterCutAngle));
	}
	float cutAngle = 13.5;
	float GetCut() {
		return glm::cos(glm::radians(12.5f));
	}

	glm::vec3 diffuseColor = glm::vec3(1.0f, 0.6196078f, 0.32941f);
	glm::vec3 specularColor = glm::vec3(1.0f, 0.780392f, 0.643137f);

	float constant = 0.20f;
	float linear = 0.030f;
	float quadratic = 0.001f;
};

struct VolLight {
	float density = 1.20f;
	float weight = 0.02f;
	float decay = 1.0f;
	float exposure = 0.45f;
	int samples = 270;
};
