//
// Created by Lavinia on 4/14/2021.
//

#include "SimpleMesh.h"
SimpleMesh::SimpleMesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices) {
    this->vertices=vertices;
    this->indices=indices;
    this->DefineMesh();
}

void SimpleMesh::DefineMesh() {
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()* sizeof(Vertex), &vertices[0],GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), &indices[0],GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof (Vertex),(GLvoid*) offsetof(Vertex,Normal));


    glBindVertexArray(0);
}
void SimpleMesh::Draw(GLuint shaderProgram) {

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
    glBindVertexArray(0);

}