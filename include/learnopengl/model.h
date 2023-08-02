/**
 * @file model.h
 * @brief Model class declaration
 * @date July 2023
 * @see https://learnopengl.com/Model-Loading/Model
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <learnopengl/shader_g.h>
#include <learnopengl/mesh.h>
#include <vector>
#include <iostream>

class Model {
public:
	Model(const char* path);
	void draw(Shader& shader);
	unsigned int getNumMeshes();
	unsigned int getMeshVAO(unsigned int meshIndex);
	unsigned int getMeshNumIndices(unsigned int meshIndex);
	unsigned int getTextureID(unsigned int textureIndex);
private:
	std::vector<Mesh> meshes;
	std::vector<Texture> texturesLoaded;
	std::string directory;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char* path, const std::string directory, bool gamma = false);
};