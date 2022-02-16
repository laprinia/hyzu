#include "../Scene.h"
#include <iostream>
#include <json.h>
#include <fstream>

Scene::Scene(const std::string& path)
{
	std::ifstream file(path, std::ifstream::binary);
	Json::Reader reader;
	Json::Value root;
	reader.parse(file, root);
	skybox=root["skybox"].asString();
	std::cout << "Loaded skybox with id: "<<skybox << std::endl;
	//TODO ADD ALL ATTRIB 
	Json::Value array = root["models"];
	for (Json::Value::ArrayIndex i = 0; i != array.size(); i++)
	{
		std::string name = array[i]["id"].asString();
		float scale = array[i]["scale"].asFloat();
		float rotate = array[i]["rotate"].asFloat();
		glm::vec3 rotateAxis{ array[i]["rotateAxis"].asString()=="x"?1:0,array[i]["rotateAxis"].asString() == "y" ? 1 : 0 , array[i]["rotateAxis"].asString() == "z" ? 1 : 0 };
		glm::vec3 translate{array[i]["translateX"].asFloat(),array[i]["translateY"].asFloat(),array[i]["translateZ"].asFloat() };
		bool hasBlend = array[i]["blendFunc"].asBool();
		ImportedModel* model = new ImportedModel(name,scale,rotate,rotateAxis,translate,hasBlend);
		models.push_back(model);
		model->ToString();
	}
}
