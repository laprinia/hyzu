//
// Created by Lavinia on 3/8/2021.
//

#ifndef HYZU_RENDERABLEOBJECT_H
#define HYZU_RENDERABLEOBJECT_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class RenderableObject {
private:
    GLuint VBO;


public:
    RenderableObject(GLfloat* vertices);
    ~RenderableObject();
};


#endif //HYZU_RENDERABLEOBJECT_H
