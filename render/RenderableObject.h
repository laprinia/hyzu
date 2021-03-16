//
// Created by Lavinia on 3/8/2021.
//

#ifndef HYZU_RENDERABLEOBJECT_H
#define HYZU_RENDERABLEOBJECT_H
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class RenderableObject {

public:
    GLuint VAO,VBO;
    RenderableObject()=default;
    void RenderObject();
    ~RenderableObject();
};


#endif //HYZU_RENDERABLEOBJECT_H
