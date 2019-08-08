#include <map>
#include <utility>

#include <glm/gtx/quaternion.hpp>

#include <Mesh.h>
#include <Animation/Joint.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

class AnimatedModel
{
public:
	Joint *rootJoint;
	Mesh mesh;
	Shader shader;
	std::vector<Joint*> joints;
	std::vector<glm::mat4> jointTransforms;
	std::map<std::string, unsigned int> names;

	void LoadModel(std::string path);
	void Cleanup();				//TODO!!!!

	glm::mat4 m_GlobalInverseTransform;

private:
	//TODOOO: method for loading an .obj file (without animations)
	Joint* processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	std::vector<Joint*> readArmature(aiMesh* mesh, const aiScene* scene, std::map<std::string, unsigned int>& names);
	
	void SetVerticesWeights(aiMesh* mesh);
	unsigned int UpdateVertexInfo(Vertex& vertex, unsigned int jointID, float weight);
	void loadAnimations(const aiScene* scene);
	void generateGlobalAnimationMatrices(Joint* joint);

	void generateMatricesForShader();

	glm::mat4 convert4x4matrix(aiMatrix4x4 matrix);
	glm::vec3 convertVec3(aiVector3D vect);

	glm::quat convertToGLMQuat(const aiQuaternion& from);
	
};
#endif	// ANIMATEDMODEL_H