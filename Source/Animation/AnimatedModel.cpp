#include <Animation/AnimatedModel.h>
#include "..\..\Include\Camera.h"

void AnimatedModel::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate| aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	Shader shader("..\\Resources\\Shaders\\CowboyVertexShader.vs", "..\\Resources\\Shaders\\CowboyFragmentShader.fs");
	this->shader = shader;
	//check if the file also contains info for animations
	//TODO: ALTER FUNCTION FOR MULTIPLE MESHES
	if (scene->HasMeshes())
	{
		aiMesh* mesh = scene->mMeshes[0];
		if (mesh->HasBones())
			joints = readArmature(mesh, scene, names);
	}


	rootJoint = processNode(scene->mRootNode, scene);
	std::cout << std::endl << "root joint name:" << rootJoint->name << std::endl;
	
	SetVerticesWeights(scene->mMeshes[0]);

	loadAnimations(scene);

	generateGlobalAnimationMatrices(rootJoint);

	jointTransforms.resize(joints.size());
	generateMatricesForShader();
}

void AnimatedModel::Cleanup()
{
	//TODO!!!!
}

Joint* AnimatedModel::processNode(aiNode* node, const aiScene* scene)
{
	std::string name = node->mName.C_Str();
	
	// we assume the file only contains one mesh
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->mesh = processMesh(mesh, scene);
	}

	if (names.count(name) > 0)
	{
		std::cout << name << " is an element of mymap.\n";

		Joint* joint = nullptr;
		unsigned int id = names.at(name);

		joint = joints[id];

		//set the parent for the joint
		if (node->mParent == NULL)
			joint->parent = nullptr;
		else
		{
			std::string parentName = node->mParent->mName.C_Str();
			if (names.count(parentName) > 0)
			{
				unsigned int parentId = names.at(name);
				joint->parent = joints[parentId];
			}
			else
				joint->parent = nullptr;
		}

		//set children
		
		//std::cout << "nr children: " << node->mNumChildren << std::endl;
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			Joint* child = processNode(node->mChildren[i], scene);

			if(child != nullptr)
				joint->children.push_back(child);
		}

		return joint;

	}
	else
	{
		std::cout << name << " IS NOT an element of mymap.\n";

		Joint* toReturn = nullptr;

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			Joint* child = processNode(node->mChildren[i], scene);

			if (child != nullptr)
				toReturn = child;
		}
		return toReturn;
	}
}

Mesh AnimatedModel::processMesh(aiMesh* mesh, const aiScene* scene)
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

		//TODO multiple textures
		//with if(mesh->HasTextureCoords(n))
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
	
	Mesh newMesh{};
	newMesh.InitMeshEBO(vertices, indices, diffuseMaps, this->shader);

	return newMesh;
}

std::vector<Texture> AnimatedModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> texturesVect;
	Texture texture;
	std::string directory = "..\\Resources\\Textures\\";

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		std::string filename;

		if (mat->GetTexture(type, i, &str) != AI_SUCCESS)
			std::cout << "[AnimatedModel] Error in loadMaterialTextures()\n";
		
		filename = directory + std::string(str.C_Str());
		texture.LoadTexture(filename.c_str());
		texture.type = typeName;

		texturesVect.push_back(texture);
	}

	return texturesVect;
}

//read the "skeleton" of the mesh (the list of bones)
std::vector<Joint*> AnimatedModel::readArmature(aiMesh* mesh, const aiScene* scene, std::map<std::string, unsigned int>& names)
{
	std::vector<Joint*> joints;

	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		Joint *currJoint = new Joint();
		aiBone* bone = mesh->mBones[i];
		std::string name = bone->mName.C_Str();

		//i is the jointID
		names.insert(std::pair <std::string, unsigned int> (name, i));

		currJoint->setInvBindPoseM(convert4x4matrix(bone->mOffsetMatrix));
		currJoint->setId(i);
		currJoint->setName(name);
		
		joints.push_back(currJoint);

		std::cout << currJoint->name << ", ";
	}
	return joints;
}

void AnimatedModel::SetVerticesWeights(aiMesh* mesh)
{
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		//i is the jointID
		aiBone* bone = mesh->mBones[i];

		for (unsigned int j = 0; j < bone->mNumWeights; j++)
		{
			aiVertexWeight var = bone->mWeights[j];
			unsigned int index = this->mesh.indices[var.mVertexId];
			//update only for joints with a significant enough value
			if (var.mWeight > 0.1f)
				UpdateVertexInfo(this->mesh.vertices[index], i, var.mWeight);
				
		}
	}
}

unsigned int AnimatedModel::UpdateVertexInfo(Vertex& vertex, unsigned int jointID, float weight)
{
	int x = vertex.auxVars.x;

	if (vertex.auxVars.x == -3.0f)
	{
		vertex.auxVars.x = weight;
		vertex.jointIds.x = jointID;
		return 0;
	}

	if (vertex.auxVars.y == -3.0f)
	{
		vertex.auxVars.y = weight;
		vertex.jointIds.y = jointID;
		return 0;
	}

	if (vertex.auxVars.z == -3.0f)
	{
		vertex.auxVars.z = weight;
		vertex.jointIds.z = jointID;
		return 0;
	}

	//if the vertex is affected by more than 3 joints, we keep the ones with a greater value (weight value is stored in auxVars)
	if(weight > vertex.auxVars.x)
	{
		vertex.auxVars.x = weight;
		vertex.jointIds.x = jointID;
		return 0;
	}

	if (weight > vertex.auxVars.y)
	{
		vertex.auxVars.y = weight;
		vertex.jointIds.y = jointID;
		return 0;
	}

	if (weight > vertex.auxVars.z)
	{
		vertex.auxVars.z = weight;
		vertex.jointIds.z = jointID;
		return 0;
	}
	
	std::cout << "[AnimatedModel::UpdateVertexInfo] ERROR, expected other values for auxVars!\n";
	return 1;
}

void AnimatedModel::loadAnimations(const aiScene* scene)
{
	if (scene->HasAnimations())
		std::cout << "has animations!!!!\n";
	std::cout << scene->mNumAnimations << std::endl;

	aiAnimation* animation = scene->mAnimations[0];
//	std::cout << "Duration:" << animation->mDuration << std::endl;
//	std::cout << "TicksPerSecond:" << animation->mTicksPerSecond << std::endl;

	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* jointAnimation = animation->mChannels[i];
		std::string jointName = jointAnimation->mNodeName.C_Str();

		unsigned int jointId = names.at(jointName);
		std::cout << "Joint " << jointName << " has index: " << jointId << std::endl;

//		std::cout << jointAnimation->mNodeName.C_Str() << " animation" << std::endl;
//		std::cout << jointAnimation->mNumPositionKeys << " mNumPositionKeys" << std::endl;
//		std::cout << jointAnimation->mPositionKeys[3].mTime << " mPositionKeys[0].mTime" << std::endl;

		glm::mat4 identity = glm::mat4(1.0f);

		aiVector3D translationVect = jointAnimation->mPositionKeys[3].mValue;
		aiVector3D scaleVect = jointAnimation->mScalingKeys[3].mValue;
		aiQuaternion rotationQuat = jointAnimation->mRotationKeys[3].mValue;
		glm::quat glmRotationQuat = convertToGLMQuat(rotationQuat);

		
		glm::mat4 translationM = glm::translate(identity, convertVec3(translationVect));
		glm::mat4 scaleM = glm::scale(identity, convertVec3(scaleVect));
		glm::mat4 rotationM = glm::toMat4(glmRotationQuat);

		glm::mat4 modelM = translationM * rotationM * scaleM;
		joints[jointId]->setLocalAnimationM(modelM);
		

	}


	//set local animation matrix
		//glm::mat4 matrix = convert4x4matrix(node->mTransformation);
		//joint->setLocalAnimationM(matrix);

	//TODO: MULTIPLE ANIMATIONS SUPPORT

}

void AnimatedModel::generateGlobalAnimationMatrices(Joint *joint)
{
	//the root joint's global animation matrix is its local animation matrix
	if(joint->parent == nullptr)
		joint->setGlobalAnimationM(joint->localAnimationM);
	else
	{
		glm::mat4 globalAnimationM = joint->parent->globalAnimationM;
		globalAnimationM = joint->localAnimationM * globalAnimationM;
		joint->setGlobalAnimationM(globalAnimationM);
	}
		
	unsigned int childCnt = joint->children.size();
	for (unsigned int i = 0; i < childCnt; i++)
	{
		generateGlobalAnimationMatrices(joint->children[i]);
	}

}

void AnimatedModel::generateMatricesForShader()
{
	for (unsigned int i = 0; i < joints.size(); i++)
	{
		glm::mat4 invBindPoseM = joints[i]->invBindPoseM;
		glm::mat4 globalAnimationM = joints[i]->globalAnimationM;
		glm::mat4 finalM = glm::transpose(globalAnimationM * invBindPoseM);

		jointTransforms[i] = finalM;
	}
}

glm::mat4 AnimatedModel::convert4x4matrix(aiMatrix4x4 matrix)
{
	glm::vec4 l1(matrix.a1, matrix.a2, matrix.a3, matrix.a4);
	glm::vec4 l2(matrix.b1, matrix.b2, matrix.b3, matrix.b4);
	glm::vec4 l3(matrix.c1, matrix.c2, matrix.c3, matrix.c4);
	glm::vec4 l4(matrix.d1, matrix.d2, matrix.d3, matrix.d4);
	
	return glm::mat4(l1, l2, l3, l4);
}

glm::vec3 AnimatedModel::convertVec3(aiVector3D vect)
{
	return glm::vec3(vect.x, vect.y, vect.z);
}

glm::quat AnimatedModel::convertToGLMQuat(const aiQuaternion& from)
{
	glm::quat to;
	to.x = from.x;
	to.y = from.y;
	to.z = from.z;
	to.w = from.w;
	return to;
}