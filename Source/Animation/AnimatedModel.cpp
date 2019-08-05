#include <Animation/AnimatedModel.h>

void AnimatedModel::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate| aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene, rootJoint);
}

void AnimatedModel::processNode(aiNode* node, const aiScene* scene, Joint& currentJoint)
{
	std::cout << "name : " << node->mName.C_Str() << std::endl;
	
	glm::mat4 matrix = convertTransfMatrix(node->mTransformation);
	
	/*if (matrix != glm::mat4(1.0f))
		std::cout << "not identity matrix!";
	std::cout << "elem: " << matrix[0][0] << std::endl;
	*/

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->mesh = processMesh(mesh, scene);
	}
	// then do the same for each of its children
	std::cout << "nr children: " << node->mNumChildren << std::endl;
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		Joint newChild{};
		processNode(node->mChildren[i], scene, newChild);
		currentJoint.children.push_back(newChild);
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
		glm::vec3 vector;

		// we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class 
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}

		//TODO multiple textures
		//with if(mesh->HasTextureCoords(n))
		if (mesh->HasTextureCoords(0))
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
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

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	
	/*
	// 2. specular maps
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

	Shader shader("..\\Resources\\Shaders\\TextureVertexShader.vs", "..\\Resources\\Shaders\\TextureFragmentShader.fs");
	
	Mesh newMesh{};
	newMesh.InitMeshEBO(vertices, indices, diffuseMaps, shader);
	// return a mesh object created from the extracted mesh data

	if (mesh->HasBones())
		readArmature(mesh, scene);

	return newMesh;
}

std::vector<Texture> AnimatedModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> texturesVect;
	Texture texture;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		if (mat->GetTexture(type, i, &str) != AI_SUCCESS)
			std::cout << "[AnimatedModel] Error in loadMaterialTextures()\n";
		

		std::string directory = "..\\Resources\\Textures\\";
		std::string filename = directory + std::string(str.C_Str());

		texture.LoadTexture(filename.c_str());
		texture.type = typeName;

		texturesVect.push_back(texture);
	}

	return texturesVect;
}

//read the "skeleton" of the mesh
void AnimatedModel::readArmature(aiMesh* mesh, const aiScene* scene)
{
	if (mesh->HasBones())
		std::cout << "has bones \n";
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		Joint currJoint;
		aiBone* bone = mesh->mBones[i];
		glm::mat4 transform = convertTransfMatrix(bone->mOffsetMatrix);

		joints.push_back(currJoint);

		currJoint.Init(joints.size() - 1, transform);
		std::cout << "Bone: " << bone->mName.C_Str() << ", ";
	}
}

glm::mat4 AnimatedModel::convertTransfMatrix(aiMatrix4x4 matrix)
{
	glm::vec4 l1(matrix.a1, matrix.a2, matrix.a3, matrix.a4);
	glm::vec4 l2(matrix.b1, matrix.b2, matrix.b3, matrix.b4);
	glm::vec4 l3(matrix.c1, matrix.c2, matrix.c3, matrix.c4);
	glm::vec4 l4(matrix.d1, matrix.d2, matrix.d3, matrix.d4);
	
	return glm::mat4(l1, l2, l3, l4);
}