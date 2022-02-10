//
// Created by Lavinia

#ifndef HYZU_RENDERABLEWITHVERTEXCOLOR_H
#define HYZU_RENDERABLEWITHVERTEXCOLOR_H
#define GLEW_STATIC
#include "RenderableObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <vector>

class RenderableWithVertexColor: public RenderableObject {

public:

    RenderableWithVertexColor()=default;
    void DefineObject(std::vector<GLfloat> vertices, std::vector<unsigned int> indices) override;
    ~RenderableWithVertexColor();
};


#endif //HYZU_RENDERABLEWITHVERTEXCOLOR_H
