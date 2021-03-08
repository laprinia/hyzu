//
// Created by Lavinia on 3/8/2021.
//

#include "ShaderManager.h"

ShaderManager &ShaderManager::GetInstance() {
    static ShaderManager instance;
    return instance;
}

void ShaderManager::CompileShader(int shaderType, std::string location) {

    GLuint shaderID = glCreateShader(shaderType);
    shaders.insert(std::pair<int, GLuint>(shaderType, shaderID));
    glShaderSource(shaderID, 1, reinterpret_cast<const GLchar *const *>(&location), nullptr);
    glCompileShader(shaderID);
}

void ShaderManager::CheckShaderCompile(int shaderType) {
    GLint success;
    GLchar log[512];
    auto shaderIterator = shaders.find(shaderType);
    glGetShaderiv(shaderIterator->second, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderIterator->second, 512, nullptr, log);
        std::cout << "Error in compiling " << shaderType << " \n" <<
                  log << std::endl;
    }

}

void ShaderManager::LinkShaderProgram() {
    shaderProgram = glCreateProgram();
    for (auto it = shaders.begin(); it != shaders.end(); ++it) {
        glAttachShader(shaderProgram, it->second);
    }
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
}

void ShaderManager::CheckShaderLink() {
    GLint success;
    GLchar log[512];
    glGetProgramiv(shaderProgram,GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram,512, nullptr,log);
    }
}

ShaderManager::~ShaderManager() {
    for (auto it = shaders.begin(); it != shaders.end(); ++it) {
        glDeleteShader(it->second);
    }
    shaders.clear();
}



