#include "Model.h"

#include "Vertex.h"
#include "TextureManager.h"
#include "Mesh.h"

#include <iostream>

Model::Model(const char* path)
{
    this->texture = true;
    this->load(path);
}

Model::Model(const char* path, TextureUVSet const& uvCoord) : uvCoord(uvCoord)
{
    this->texture = false;
    this->load(path);
}

Model::~Model()
{
    for (unsigned int i = 0; i < meshes.size(); ++i) {
        delete meshes[i];
    }
    meshes.clear();
}

void Model::render(const Shader* shader) const
{
    for (unsigned int i = 0; i < this->meshes.size(); ++i) {
        this->meshes[i]->render(shader);
    }
}

const Mesh* Model::getMesh(int ind) const
{
    return this->meshes[ind];
}

std::vector<Mesh *> const &Model::getMeshes() const
{
    return this->meshes;
}

void Model::load(std::string const& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Cannot load model." << std::endl;
        return;
    }

    this->dir = path.substr(0, path.find_last_of('/'));

    this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        this->processNode(node->mChildren[i], scene);
    }
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture const*> textures;

    float uScale = 1.0f, vScale = 1.0f;

    if (!(this->texture))
    {
        uScale = uvCoord.coords[1].x - uvCoord.coords[2].x;
        vScale = uvCoord.coords[1].y - uvCoord.coords[0].y;
    }
    

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

        if (mesh->HasNormals())
        {
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
        }

        if (mesh->mTextureCoords[0]) {
            vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = mesh->mTextureCoords[0][i].y;

            if (!(this->texture))
            {
                vertex.texCoord.x = uvCoord.coords[2].x + vertex.texCoord.x * uScale;
                vertex.texCoord.y = uvCoord.coords[2].y + vertex.texCoord.y * vScale;
            }
        } else {
            vertex.texCoord.x = vertex.texCoord.y = 0.0f;
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    if (this->texture)
        textures = loadMaterialTexture(material, aiTextureType_DIFFUSE);

    return new Mesh(vertices, indices, textures);
}

std::vector<Texture const*> Model::loadMaterialTexture(aiMaterial* material, aiTextureType type)
{
    std::vector<Texture const*> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
        aiString str;
        material->GetTexture(type, i, &str);
        std::string path = dir + "/" + str.C_Str();

        textures.push_back(TextureManager::getTexture(path.c_str()));
    }
    return textures;
}
