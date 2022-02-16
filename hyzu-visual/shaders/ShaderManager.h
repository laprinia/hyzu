//
// Created by Lavinia
//

#ifndef HYZU_SHADERMANAGER_H
#define HYZU_SHADERMANAGER_H
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <map>
#include <fstream>

class ShaderManager {
private:
    static std::map<int, GLuint> shaders;
    static std::string ReadFromFile(const std::string &location);

    ShaderManager() = default;

public:


    static ShaderManager &GetInstance();
    
    static GLuint AddShader(const std::string& startName);

    static void CompileShader(int shaderType, const std::string &location);

    static void CheckShaderCompile(int shaderType);

    static GLuint LinkShaderProgram();

    static void CheckShaderLink(GLuint shaderProgram);
    

    ~ShaderManager();
};


#endif //HYZU_SHADERMANAGER_H
