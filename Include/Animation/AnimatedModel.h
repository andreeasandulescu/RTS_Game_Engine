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
	Joint rootJoint;
	Mesh mesh;
	std::vector<Joint> joints;

	void LoadModel(std::string path);

private:
	void processNode(aiNode* node, const aiScene* scene, Joint& currentJoint);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	void readArmature(aiMesh* mesh, const aiScene* scene);
	glm::mat4 convertTransfMatrix(aiMatrix4x4 matrix);
	
};
#endif	// ANIMATEDMODEL_H