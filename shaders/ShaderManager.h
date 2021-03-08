//
// Created by Lavinia on 3/8/2021.
//

#ifndef HYZU_SHADERMANAGER_H
#define HYZU_SHADERMANAGER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <map>

class ShaderManager {
private:
    static GLuint shaderProgram;
    static std::map<int, GLuint> shaders;
    ShaderManager();
    ~ShaderManager();
public:
    static ShaderManager& GetInstance();
    static void CompileShader(int shaderType, std::string location);
    static void CheckShaderCompile(int shaderType);
    static void LinkShaderProgram();
    static void CheckShaderLink();
};


#endif //HYZU_SHADERMANAGER_H
