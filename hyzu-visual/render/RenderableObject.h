//
// Created by Lavinia

#ifndef HYZU_RENDERABLEOBJECT_H
#define HYZU_RENDERABLEOBJECT_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
class RenderableObject {
public:
    GLuint VAO, VBO, EBO;

    virtual ~RenderableObject() = default;

    virtual void DefineObject(std::vector<GLfloat> vertices, std::vector<unsigned int> indices) = 0;
};


#endif //HYZU_RENDERABLEOBJECT_H
