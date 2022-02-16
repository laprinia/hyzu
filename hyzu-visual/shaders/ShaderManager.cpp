//
// Created by Lavinia
// 
//


#include "ShaderManager.h"

std::map<int, GLuint> ShaderManager::shaders = {};


ShaderManager& ShaderManager::GetInstance() {
	static ShaderManager instance;
	return instance;
}

GLuint ShaderManager::AddShader(const std::string& startName)
{
	std::string vertexsuff = "VS.glsl";
	ShaderManager::CompileShader(GL_VERTEX_SHADER, ".\\..\\hyzu-visual\\shaders\\" + startName + vertexsuff);
	ShaderManager::CheckShaderCompile(GL_VERTEX_SHADER);

	std::string fragsuff = "FS.glsl";
	ShaderManager::CompileShader(GL_FRAGMENT_SHADER, ".\\..\\hyzu-visual\\shaders\\" + startName + fragsuff);
	ShaderManager::CheckShaderCompile(GL_FRAGMENT_SHADER);

	GLuint shaderProgram = ShaderManager::LinkShaderProgram();
	ShaderManager::CheckShaderLink(shaderProgram);
	return shaderProgram;
}

std::string ShaderManager::ReadFromFile(const std::string& location) {
	std::string fileContent;
	std::ifstream fileStream(location, std::ios::in);
	if (!fileStream.is_open()) {
		std::cerr << " Could not find file " << location << " ." << std::endl;
		return "";
	}
	std::string line;
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		fileContent.append(line + "\n");
	}
	fileStream.close();
	return fileContent;
}

void ShaderManager::CompileShader(int shaderType, const std::string& location) {

	GLuint shaderID = glCreateShader(shaderType);
	shaders.insert(std::pair<int, GLuint>(shaderType, shaderID));
	std::string shaderString = ReadFromFile(location);
	const char* shaderChar = shaderString.c_str();
	glShaderSource(shaderID, 1, &shaderChar, nullptr);
	glCompileShader(shaderID);
}

void ShaderManager::CheckShaderCompile(int shaderType) {
	GLint success;
	GLchar log[512];
	auto shaderIterator = shaders.find(shaderType);
	glGetShaderiv(shaderIterator->second, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderIterator->second, 512, nullptr, log);
		std::cout << "Error in compiling " << shaderType << " \n" << std::endl;
		std::cout << log << std::endl;
	}
	else {
		std::cout << "Success in compiling " << shaderType << " \n" << std::endl;
	}

}

GLuint ShaderManager::LinkShaderProgram() {
	GLuint shaderProgram = glCreateProgram();
	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		glAttachShader(shaderProgram, it->second);
	}
	glLinkProgram(shaderProgram);
	return shaderProgram;
}

void ShaderManager::CheckShaderLink(GLuint shaderProgram) {
	GLint success;
	GLchar log[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, log);
	}
	for (auto it = shaders.begin(); it != shaders.end(); ++it) {
		glDeleteShader(it->second);
	}
	shaders.clear();
}

ShaderManager::~ShaderManager() = default;