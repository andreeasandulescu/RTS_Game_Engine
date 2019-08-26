#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include<Shader.h>

#ifndef MESH_H
#define MESH_H

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec4 auxVars;			//for the animated model, we will store the weights here
	glm::uvec4 jointIds;		//the jointIds for the animated models		

	Vertex();
	Vertex& operator=(const Vertex& v);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, glm::vec4 auxVars);
	Vertex(glm::vec3 position, glm::vec4 auxVars);
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
	Texture& operator=(const Texture& t);
	void LoadTexture(const GLchar* texturePath);
};

class Mesh
{
public:
	//beware, the EBO is only used in methods containg the word "EBO" (e.g.  InitMeshEBO() )
	unsigned int VAO, VBO, EBO, instanceVBO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> translationVects;
	std::vector<Texture> textures;
	Shader shader;
	
	//init functions
	//------------------
	
	void InitMesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const Shader& shader);
	void InitMesh(const std::vector<Vertex>& vertices, const Shader& shader);

	//for the EBO implementation
	void InitMeshEBO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, const Shader& shader);
	void InitMeshEBO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader);

	//for instanced rendering, does not use EBO
	void InitMeshInstanced(const std::vector<Vertex>& vertices, std::vector<glm::vec3> &translationVects, const std::vector<Texture>& textures, const Shader& shader);
	void InitMeshInstanced(const std::vector<Vertex>& vertices, std::vector<glm::vec3> &translationVects, const Shader& shader);


	//create functions
	//---------------------
	void createNewMesh();
	void createNewMeshEBO();
	void createNewMeshInstanced();

	//update functions
	//----------------------
	void UpdateMesh();
	void UpdateMeshEBO();
	void UpdateMeshInstanced();

	//draw functions
	//-----------------------
	void Draw(GLenum mode);
	void Draw(const glm::mat4& transform, GLenum mode);
	
	//for the EBO implementation
	void DrawEBO(GLenum mode);
	void DrawEBO(const glm::mat4& transform, GLenum mode);

	void DrawInstanced(const glm::mat4& transform, GLenum mode);

	//cleanup functions
	//-------------------------
	void Cleanup();
	void CleanupEBO();

	//utilities functions
	//-------------------------------
	Mesh& operator=(const Mesh& m);
	void copyFromVBO();

};
#endif	// MESH_H