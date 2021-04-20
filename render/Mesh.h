//
// Created by Lavinia on 4/14/2021.
//

#ifndef HYZU_MESH_H
#define HYZU_MESH_H
#define GLEW_STATIC

#include "RenderableObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <assimp/types.h>


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoords;
    glm::vec3 Tangent;
};

struct Texture {
    GLuint id;
    std::string type;
    aiString path;
};

class Mesh {
private:
    GLuint VAO, VBO, EBO;

    void DefineMesh();

public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures);

    void Draw(GLuint shaderProgram);

};


#endif //HYZU_MESH_H
