#include "../Scene.h"

Scene::Scene(const std::string& path)
{
	std::ifstream file(path, std::ifstream::binary);
	Json::Reader reader;
	Json::Value root;
	reader.parse(file, root);
	skybox = root["skybox"].asString();
	Json::Value array = root["models"];
	for (Json::Value::ArrayIndex i = 0; i <array.size(); i++)
	{

		std::string name = array[i]["id"].asString();
		float scale = array[i]["scale"].asFloat();
		float rotate = array[i]["rotate"].asFloat();
		glm::vec3 rotateAxis{ array[i]["rotateAxis"].asString() == "x" ? 1 : 0,array[i]["rotateAxis"].asString() == "y" ? 1 : 0 , array[i]["rotateAxis"].asString() == "z" ? 1 : 0 };
		glm::vec3 translate{ array[i]["translateX"].asFloat(),array[i]["translateY"].asFloat(),array[i]["translateZ"].asFloat() };
		bool hasBlend = array[i]["blendFunc"].asBool();
		bool isVisible = array[i]["visible"].asBool();

		std::vector<ImportedModel> subItems;
		if (array[i].isObject() && array[i].isMember("models")) {

			Json::Value subArray = array[i]["models"];
			for (Json::Value::ArrayIndex j = 0; j != subArray.size(); j++)
			{

				std::string name = subArray[j]["id"].asString();
				float scale = subArray[j]["scale"].asFloat();
				float rotate = subArray[j]["rotate"].asFloat();
				glm::vec3 rotateAxis{ subArray[j]["rotateAxis"].asString() == "x" ? 1 : 0,subArray[j]["rotateAxis"].asString() == "y" ? 1 : 0 , subArray[j]["rotateAxis"].asString() == "z" ? 1 : 0 };
				glm::vec3 translate{ subArray[j]["translateX"].asFloat(),subArray[j]["translateY"].asFloat(),subArray[j]["translateZ"].asFloat() };
				bool hasBlend = subArray[j]["blendFunc"].asBool();
				bool isVisible = subArray[j]["visible"].asBool();
				ImportedModel subModel(name, scale, rotate, rotateAxis, translate, hasBlend,subItems, isVisible);
				subItems.push_back(subModel);
			}
		}
		ImportedModel* model = new ImportedModel(name, scale, rotate, rotateAxis, translate, hasBlend, subItems, isVisible);
		models.push_back(model);
	}
}