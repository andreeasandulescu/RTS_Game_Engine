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
	glm::vec3 auxVars;
	glm::ivec3 weights;						//for the animated models
	Vertex& operator=(const Vertex& v);
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
	unsigned int VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Shader shader;
	
	void InitMesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const Shader& shader);
	void InitMesh(const std::vector<Vertex>& vertices, const Shader& shader);
	
	//for the EBO implementation
	void InitMeshEBO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, const Shader& shader);
	void InitMeshEBO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader);

	
	Mesh& operator=(const Mesh& m);
	void UpdateMesh();
	void Draw(GLenum mode);
	void Draw(const glm::mat4& transform, GLenum mode);
	void Cleanup();

	//for the EBO implementation
	void UpdateMeshEBO();
	void DrawEBO(GLenum mode);
	void DrawEBO(const glm::mat4& transform, GLenum mode);
	void CleanupEBO();

private:
	void createNewMesh();

	//for the EBO implementation
	void createNewMeshEBO();
};
#endif	// MESH_H