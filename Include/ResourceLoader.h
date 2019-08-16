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

	Mesh* LoadModel(std::string path, Shader shader);
	AnimatedModel* LoadAnimatedModel(std::string path, Shader shader);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	
	//utilities functions
	glm::mat4 convert4x4matrix(aiMatrix4x4 matrix);
	glm::vec3 convertVec3(aiVector3D vect);

	void Cleanup();				//TODO!!!!


private:
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene, Shader& shader);
	Joint* createJointHierarchy(aiNode* node, const aiScene* scene, AnimatedModel *animModel);
	void readBonesList(aiMesh* mesh, const aiScene* scene, AnimatedModel *animModel);
	void SetVerticesWeights(aiMesh* mesh, AnimatedModel *animModel);
	void UpdateVertexInfo(Vertex& vertex, unsigned int jointID, float weight);
	void loadAnimations(const aiScene* scene, AnimatedModel *animModel);
	void generateGlobalAnimationMatrices(Joint* joint);
	void runAnimation();
	void setFinalJointTransforms(AnimatedModel *animModel);
	void readMeshes(const aiScene* scene, aiNode* node, std::vector<Mesh*>& meshesVector, Shader& shader);	//use to add support for multiple vertices in the future
};
#endif	// RESOURCELOADER_H