//
// Created by Lavinia
//

#ifndef HYZU_SIMPLEMESH_H
#define HYZU_SIMPLEMESH_H
#define GLEW_STATIC

#include "RenderableObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <vector>
#include <sstream>

struct SVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};


class SimpleMesh {
private:
    GLuint VAO, VBO, EBO;

    void DefineMesh();

public:
    std::vector<SVertex> vertices;
    std::vector<GLuint> indices;

    SimpleMesh(const std::vector<SVertex> &vertices, const std::vector<GLuint> &indices);

    void Draw(GLuint shaderProgram);

};


#endif //HYZU_SIMPLEMESH_H
