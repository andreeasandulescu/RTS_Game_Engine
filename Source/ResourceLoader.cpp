#include <ResourceLoader.h>

#include <Animation/AnimatedModel.h>
#include "..\Include\Camera.h"

Mesh* ResourceLoader::LoadModel(std::string path, Shader shader)
{
	const aiScene* scene = assimpImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << assimpImporter.GetErrorString() << std::endl;
		return nullptr;
	}

	Mesh *mesh = processMesh(scene->mMeshes[0], scene, shader);
	return mesh;
}

Mesh* ResourceLoader::processMesh(aiMesh* mesh, const aiScene* scene, Shader& shader)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		//retrieve vertex position from the aiVector3D variable
		vertex.position = convertVec3(mesh->mVertices[i]);

		if (mesh->HasNormals())
			vertex.normal = convertVec3(mesh->mNormals[i]);
		else
			std::cout << "Mesh normals not defined in file!" << std::endl;

		if (mesh->HasTextureCoords(0))
		{
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		vertices.push_back(vertex);
	}

	// retrieve the vertex indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		if (face.mNumIndices != 3)
			std::cout << face.mNumIndices << "Primitive is not a triangle! Please use only triangles\n";

		if (face.mNumIndices == 3)
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
	}

	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	/*
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	*/

	//Texture diffuseTexture{};
	//diffuseTexture.LoadTexture("..\\Resources\\Textures\\diffuse.png");
	//std::vector<Texture> textVect{ diffuseTexture };

	Mesh *newMesh = new Mesh();
	newMesh->InitMeshEBO(vertices, indices, diffuseMaps, shader);

	return newMesh;
}

std::vector<Texture> ResourceLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> texturesVect;
	Texture texture;
	std::string directory = "..\\Resources\\Textures\\";

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		std::string filename;

		if (mat->GetTexture(type, i, &str) != AI_SUCCESS)
			std::cout << "[ResourceLoader] Error in loadMaterialTextures()\n";

		filename = directory + std::string(str.C_Str());
		texture.LoadTexture(filename.c_str());
		texture.type = typeName;

		texturesVect.push_back(texture);
	}

	return texturesVect;
}

//returns the root node of the hierarchy
Joint* ResourceLoader::createJointHierarchy(aiNode* node, const aiScene* scene, AnimatedModel *animModel)
{
	std::string nodeName = node->mName.C_Str();

	if (animModel->names.count(nodeName) > 0)
	{
		//add node to hierarchy
		unsigned int id = animModel->names.at(nodeName);
		Joint* joint = animModel->joints[id];;

		//set the parent for the joint
		if (node->mParent == NULL)
			joint->parent = nullptr;
		else
		{
			std::string parentName = node->mParent->mName.C_Str();
			if (animModel->names.count(parentName) > 0)
			{
				unsigned int parentId = animModel->names.at(parentName);
				joint->parent = animModel->joints[parentId];
			}
			else
				joint->parent = nullptr;
		}

		//set children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			Joint* child = createJointHierarchy(node->mChildren[i], scene, animModel);
			if (child != nullptr)
				joint->children.push_back(child);
		}

		return joint;

	}
	else
	{
		//do not add node to hierarchy
		Joint* toReturn = nullptr;

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			Joint* child = createJointHierarchy(node->mChildren[i], scene, animModel);
			if (child != nullptr)
				toReturn = child;
		}
		return toReturn;
	}
}

//read the "skeleton" of the mesh (the list of bones)
void ResourceLoader::readBonesList(aiMesh* mesh, const aiScene* scene, AnimatedModel *animModel)
{
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		Joint* currJoint = new Joint();
		aiBone* bone = mesh->mBones[i];
		std::string name = bone->mName.C_Str();

		//i is the jointID
		animModel->names.insert(std::pair <std::string, unsigned int>(name, i));

		currJoint->setInvBindPoseM((convert4x4matrix(bone->mOffsetMatrix)));
		currJoint->setId(i);
		currJoint->setName(name);

		animModel->joints.push_back(currJoint);
	}
}

void ResourceLoader::SetVerticesWeights(aiMesh* mesh, AnimatedModel *animModel)
{
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		//i is the jointID
		aiBone* bone = mesh->mBones[i];
		unsigned int id = animModel->names.at(bone->mName.C_Str());

		for (unsigned int j = 0; j < bone->mNumWeights; j++)
		{
			aiVertexWeight var = bone->mWeights[j];
			unsigned int index = animModel->mesh.indices[var.mVertexId];
			//update only for joints with a significant enough value
			if (var.mWeight > 0.01f)
				UpdateVertexInfo(animModel->mesh.vertices[index], id, var.mWeight);
		}
	}
	animModel->mesh.UpdateMeshEBO();
}

void ResourceLoader::UpdateVertexInfo(Vertex& vertex, unsigned int jointID, float weight)
{
	int x = vertex.auxVars.x;

	for (unsigned int i = 0; i < 4; i++)
	{
		if (vertex.auxVars[i] < 0.0f)
		{
			vertex.auxVars[i] = weight;
			vertex.jointIds[i] = jointID;
			return;
		}
	}

	//if the vertex is affected by more than 4 joints, we keep the ones with a greater value (weight value is stored in auxVars)
	for (unsigned int i = 0; i < 4; i++)
	{
		if (weight > vertex.auxVars[i])
		{
			vertex.auxVars[i] = weight;
			vertex.jointIds[i] = jointID;
			return;
		}
	}
}

void ResourceLoader::loadAnimations(const aiScene* scene, AnimatedModel *animModel)
{
	if (scene->HasAnimations())
		std::cout << "has animations!!!!\n";

	if(scene->mNumAnimations > 1)
		std::cout << "Only one animation is supported!" <<std::endl;
	
	aiAnimation* animation = scene->mAnimations[0];
	animModel->ticksPerSecond = (float)animation->mTicksPerSecond;
	animModel->durationInTicks = (float)animation->mDuration;

	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* jointAnimation = animation->mChannels[i];
		std::string jointName = jointAnimation->mNodeName.C_Str();
		unsigned int jointId = animModel->names.at(jointName);

		int keyframeCnt = jointAnimation->mNumPositionKeys;

		if (jointAnimation->mNumPositionKeys != jointAnimation->mNumRotationKeys || jointAnimation->mNumPositionKeys != jointAnimation->mNumScalingKeys)
			std::cout << "Error in loadAnimation, mNumPositionKeys != mNumRotationKeys || mNumPositionKeys != mNumScalingKeys\n";

		for (unsigned int j = 0; j < keyframeCnt; j++)
		{
			float timeInTicks = (float)jointAnimation->mPositionKeys[j].mTime;

			glm::vec3 translationVect = convertVec3(jointAnimation->mPositionKeys[j].mValue);
			glm::vec3 scaleVect = convertVec3(jointAnimation->mScalingKeys[j].mValue);
			aiQuaternion rotationQuat = jointAnimation->mRotationKeys[j].mValue;

			Keyframe* currKeyframe = new Keyframe(translationVect, scaleVect, rotationQuat, timeInTicks);
			animModel->joints[jointId]->keyframes.push_back(currKeyframe);
		}
	}
}

AnimatedModel* ResourceLoader::LoadAnimatedModel(std::string path, Shader shader)
{
	const aiScene* scene = assimpImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << assimpImporter.GetErrorString() << std::endl;
		return nullptr;
	}

	if (!scene->HasAnimations())
	{
		std::cout << "Error! File does not contain animations!" << std::endl;
		return nullptr;
	}

	AnimatedModel* animModel = new AnimatedModel();
	aiMesh *auxMesh = scene->mMeshes[0];

	glm::mat4 globalInverseTransform = convert4x4matrix(scene->mRootNode->mTransformation);
	animModel->globalInverseTransform = glm::inverse(globalInverseTransform);

	Mesh *mesh = processMesh(auxMesh, scene, shader);
	animModel->mesh = *mesh;

	readBonesList(auxMesh, scene, animModel);

	animModel->rootJoint = createJointHierarchy(scene->mRootNode, scene, animModel);
	
	SetVerticesWeights(auxMesh, animModel);
	loadAnimations(scene, animModel);

	return animModel;
}


void ResourceLoader::readMeshes(const aiScene* scene, aiNode* node, std::vector<Mesh*>& meshesVector, Shader& shader)
{
	std::string nodeName = node->mName.C_Str();

	// we assume the file only contains one mesh
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* auxMesh = scene->mMeshes[node->mMeshes[i]];
		Mesh *mesh = processMesh(auxMesh, scene, shader);
		meshesVector.push_back(mesh);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
		readMeshes(scene, node->mChildren[i], meshesVector, shader);
}

glm::mat4 ResourceLoader::convert4x4matrix(aiMatrix4x4 matrix)
{
	glm::vec4 l1(matrix.a1, matrix.a2, matrix.a3, matrix.a4);
	glm::vec4 l2(matrix.b1, matrix.b2, matrix.b3, matrix.b4);
	glm::vec4 l3(matrix.c1, matrix.c2, matrix.c3, matrix.c4);
	glm::vec4 l4(matrix.d1, matrix.d2, matrix.d3, matrix.d4);

	return glm::mat4(l1, l2, l3, l4);
}

glm::vec3 ResourceLoader::convertVec3(aiVector3D vect)
{
	return glm::vec3(vect.x, vect.y, vect.z);
}