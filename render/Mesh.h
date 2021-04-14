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

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 TextureCoords;
};

struct Texture {
    GLuint id;
    std::string type;
};

class Mesh {
private:
    GLuint VAO,VBO,EBO;
    void DefineMesh();
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures);
    void Draw(GLuint shaderProgram );

};


#endif //HYZU_MESH_H
