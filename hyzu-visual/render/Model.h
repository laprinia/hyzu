//
// Created by Lavinia
//

#ifndef HYZU_MODEL_H
#define HYZU_MODEL_H
#define GLEW_STATIC

#include "Model.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../render/Mesh.h"
#include "SOIL.h"

class Model {
public:
    Model(const std::string &path);

    void Draw(GLuint shaderProgram);

private:
    std::vector<Mesh> meshes;
    std::vector<Texture> texturesLoaded;
    std::string modelDirectory;

    void LoadModel(const std::string &modelPath);

    void ProcessNode(aiNode *node, const aiScene *scene);

    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial *material, aiTextureType type, const std::string &typeName);

    static GLint TextureFromFile(const char *path, const std::string &directory, bool isDiffuse);
};


#endif //HYZU_MODEL_H
