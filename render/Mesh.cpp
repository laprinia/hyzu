//
// Created by Lavinia on 4/14/2021.
//


#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices,
           const std::vector<Texture> &textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->DefineMesh();
}

void Mesh::DefineMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, TextureCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent));

    glBindVertexArray(0);
}

void Mesh::Draw(GLuint shaderProgram) {
    GLuint diffuseNumber = 1;
    GLuint specularNumber = 1;
    GLuint normalNumber = 1;
    GLuint heightNumber = 1;

    for (GLuint i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::stringstream stringstream;
        std::string finalNumberString;
        std::string textureType = textures[i].type;
        if (textureType == "texture_diffuse") {
            stringstream << diffuseNumber++;
        } else if (textureType == "texture_specular") {
            stringstream << specularNumber++;
        }
         else if (textureType == "texture_normal") {
            stringstream << normalNumber++;
        }
         else if (textureType == "texture_height") {
            stringstream << heightNumber++;
        }

        finalNumberString = stringstream.str();

        glUniform1i(glGetUniformLocation(shaderProgram, (textureType + finalNumberString).c_str()),i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);

    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}
