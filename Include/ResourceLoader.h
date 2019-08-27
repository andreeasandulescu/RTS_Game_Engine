#include <map>
#include <utility>
#include <math.h>
#include <algorithm>

#include <Mesh.h>
#include <Animation/Joint.h>
#include <Animation/AnimatedModel.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef RESOURCELOADER_H
#define RESOURCELOADER_H

class ResourceLoader
{
public:
	Assimp::Importer assimpImporter;

	// retrieve shader by name:
	Shader getShader(std::string name);

	// retrieve texture by name:
	Texture getTexture(std::string name);

	Mesh* LoadModel(std::string path, Shader shader);
	AnimatedModel* LoadAnimatedModel(std::string path, Shader shader);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	
	//utilities functions
	glm::mat4 convert4x4matrix(aiMatrix4x4 matrix);
	glm::vec3 convertVec3(aiVector3D vect);

	// load all resources
	// (called in opengl context)
	void loadResources();

	void Cleanup();				//TODO!!!!

private:
	// map with key = name value = texture object:
	std::map<std::string, Texture> textures;

	// map with key = name value = shader object
	std::map<std::string, Shader> shaders;

	// load all textures
	void loadTextures();
	
	// load all shaders
	void loadShaders();

	

	Mesh* processMesh(aiMesh* mesh, const aiScene* scene, Shader& shader);
	Joint* createJointHierarchy(aiNode* node, const aiScene* scene, AnimatedModel *animModel);
	void readBonesList(aiMesh* mesh, const aiScene* scene, AnimatedModel *animModel);
	void SetVerticesWeights(aiMesh* mesh, AnimatedModel *animModel);
	void UpdateVertexInfo(Vertex& vertex, unsigned int jointID, float weight);
	void loadAnimations(const aiScene* scene, AnimatedModel *animModel);

	void readMeshes(const aiScene* scene, aiNode* node, std::vector<Mesh*>& meshesVector, Shader& shader);	//use to add support for multiple vertices in the future
};
#endif	// RESOURCELOADER_H