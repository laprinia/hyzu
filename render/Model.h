//
// Created by Lavinia on 4/14/2021.
//

#ifndef HYZU_MODEL_H
#define HYZU_MODEL_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../render/Mesh.h"

class Model {
public:
    Model(GLchar *path);

    void Draw(GLuint shaderProgram);

private:
    std::vector<Mesh> meshes;
    std::string modelDirectory;

    void LoadModel(const std::string& modelPath);

    void ProcessNode(aiNode *node, const aiScene *scene);

    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> LoadMaterialTexture(aiMaterial *material, aiTextureType type, std::string typeName);

    GLint TextureFromFile(const char* path, std::string directory);
};


#endif //HYZU_MODEL_H
