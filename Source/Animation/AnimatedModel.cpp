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

	std::cout << "nr children: " << scene->mRootNode->mNumChildren << std::endl;
	std::cout << "nr animations: " << scene->mNumAnimations << std::endl;

	processNode(scene->mRootNode, scene, jointHierarchy);


}

void AnimatedModel::processNode(aiNode* node, const aiScene* scene, Joint& currentJoint)
{
	// process all the node’s meshes
	std::cout << "nr meshes: " << node->mNumMeshes << std::endl;
	std::cout << "name : " << node->mName.C_Str() << std::endl;
	
	glm::mat4 matrix = convertTransfMatrix(node->mTransformation);
	
	/*if (matrix != glm::mat4(1.0f))
		std::cout << "not identity matrix!";
	std::cout << "elem: " << matrix[0][0] << std::endl;
	*/

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		currentJoint.meshes.push_back(processMesh(mesh, scene));
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

	// Walk through each of the mesh's vertices
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

/*

		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertices.push_back(vertex);
*/
	}

	
	// retrieve the vertex indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		if (face.mNumIndices != 3)
			std::cout << face.mNumIndices << " nr of indices not 3, cannot draw triangle\n";

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
	return newMesh;
}


std::vector<Texture> AnimatedModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	unsigned int j = mat->GetTextureCount(type);
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		if (mat->GetTexture(type, i, &str) == AI_SUCCESS)
			std::cout << "success \n";
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
	
		/*
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		*/

		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;

		//	std::string directory = path.substr(0, path.find_last_of('/'));
			std::string filename = std::string(str.C_Str());
		//	filename = this->directory + '/' + filename;

			//texture.LoadTexture(filename.c_str());
			texture.LoadTexture("..\\Resources\\Models\\diffuse.png");
			
			texture.type = typeName;
			texture.path = str.C_Str();


			textures.push_back(texture);
			//textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

glm::mat4 AnimatedModel::convertTransfMatrix(aiMatrix4x4 matrix)
{
	glm::vec4 l1(matrix.a1, matrix.a2, matrix.a3, matrix.a4);
	glm::vec4 l2(matrix.b1, matrix.b2, matrix.b3, matrix.b4);
	glm::vec4 l3(matrix.c1, matrix.c2, matrix.c3, matrix.c4);
	glm::vec4 l4(matrix.d1, matrix.d2, matrix.d3, matrix.d4);
	
	return glm::mat4(l1, l2, l3, l4);
}