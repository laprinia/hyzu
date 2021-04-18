//
// Created by Lavinia on 4/14/2021.
//

#include "Model.h"

Model::Model(GLchar *path) {
    LoadModel(path);
}

void Model::Draw(GLuint shaderProgram) {
    for (auto & mesh : meshes) {
        mesh.Draw(shaderProgram);
    }
}

void Model::LoadModel(const std::string &modelPath) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(modelPath,
                                             aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

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
        vertices.push_back(vertex);
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vector2 = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.TextureCoords = vector2;
        } else {
            vertex.TextureCoords = glm::vec2(0.0f, 0.0f);
        }
    }
    for (GLuint i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (GLuint j = 0; j < mesh->mNumFaces; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTexture(aiMaterial *material, aiTextureType type, const std::string& typeName) {

    std::vector<Texture> textures;
    for (GLuint i = 0; i < material->GetTextureCount(type); i++) {
        aiString string;
        aiGetMaterialTexture(material, type, i, &string);
        bool skip=false;
        for(GLuint j=0;j<texturesLoaded.size(); j++){
            if(texturesLoaded[j].path==string) {
                textures.push_back(texturesLoaded[j]);
                skip=true;
                break;
            }
        }
        if(!skip) {
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

GLint Model::TextureFromFile(const char *path, const std::string& directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}
