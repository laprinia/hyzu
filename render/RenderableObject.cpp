//
// Created by Lavinia on 3/8/2021.
//

#include "RenderableObject.h"

RenderableObject::RenderableObject(GLfloat *vertices) {

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}


RenderableObject::~RenderableObject() {

}

