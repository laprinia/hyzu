//
// Created by Lavinia on 4/14/2021.
//

#include <stb_image_aug.h>
#include "Model.h"

Model::Model(GLchar *path) {
    LoadModel(path);
}

void Model::Draw(GLuint shaderProgram) {
    for (auto &mesh : meshes) {
        mesh.Draw(shaderProgram);
    }
}

void Model::LoadModel(const std::string &modelPath) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(modelPath,
                                             aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals |
                                             aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Assimp Error: " << importer.GetErrorString() << std::endl;
        return;
    }
    modelDirectory = modelPath.substr(0, modelPath.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene) {
    for (GLuint i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    for (GLuint i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }

}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    for (GLuint i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Position = vector;
        vector = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.Normal = vector;
        vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vector2 = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.TextureCoords = vector2;
        } else {
            vertex.TextureCoords = glm::vec2(0.0f, 0.0f);

        }
        vertices.push_back(vertex);
    }
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    if (mesh->mMaterialIndex >= 0) {

        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::vector<Texture> roughMaps = LoadMaterialTextures(material, aiTextureType_SHININESS, "texture_shininess");
        textures.insert(textures.end(), roughMaps.begin(), roughMaps.end());
        std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }
    return Mesh(vertices, indices, textures);
}

std::vector<Texture>
Model::LoadMaterialTextures(aiMaterial *material, aiTextureType type, const std::string &typeName) {

    std::vector<Texture> textures;
    std::cout << typeName << " found: " << material->GetTextureCount(type) << std::endl;
    for (GLuint i = 0; i < material->GetTextureCount(type); i++) {

        aiString string;
        material->GetTexture(type, i, &string);
        bool skip = false;
        for (GLuint j = 0; j < texturesLoaded.size(); j++) {
            if (std::strcmp(texturesLoaded[j].path.data, string.C_Str()) == 0) {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {

            Texture texture;
            texture.id = TextureFromFile(string.C_Str(), modelDirectory);
            texture.type = typeName;
            texture.path = string;
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }
    return textures;
}

GLint Model::TextureFromFile(const char *path, const std::string &directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
