#include <map>
#include <utility>
#include <math.h>
#include <algorithm>

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

	float startTime = -1.0f;		//of the animation
	float durationInTicks;
	float ticksPerSecond;

	glm::mat4 m_GlobalInverseTransform;

	void LoadModel(std::string path);
	void runAnimation();					//time elapsed since the first command to run the animation (seconds)
	void Draw(bool animationActive, glm::mat4 transform);
	void Cleanup();				//TODO!!!!


private:
	//TODOOO: method for loading an .obj file (without animations)
	Joint* processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	std::vector<Joint*> readArmature(aiMesh* mesh, const aiScene* scene, std::map<std::string, unsigned int>& names);
	
	void SetVerticesWeights(aiMesh* mesh);
	void UpdateVertexInfo(Vertex& vertex, unsigned int jointID, float weight);
	void loadAnimations(const aiScene* scene);
	void generateGlobalAnimationMatrices(Joint* joint);
	void generateMatricesForShader();

	glm::mat4 convert4x4matrix(aiMatrix4x4 matrix);
	glm::vec3 convertVec3(aiVector3D vect);
};
#endif	// ANIMATEDMODEL_H