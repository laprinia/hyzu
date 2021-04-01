//
// Created by Lavinia on 3/8/2021.
//

#include "RenderableObject.h"


void RenderableObject::DefineObject() {
    GLfloat  vertices[] = {
            0.5f,  0.5f, 0.0f,  1.0f,0.0f,0.0f,
            0.5f, -0.5f, 0.0f,  0.0f, 1.0f,0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f,1.0f,0.0f
    };
    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

}

RenderableObject::~RenderableObject() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}



