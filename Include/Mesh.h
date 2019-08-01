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
	Vertex& operator=(const Vertex& v);
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
	Texture& operator=(const Texture& t);
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Shader shader;
	
	Mesh();
	void InitMesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const Shader& shader);
	void InitMesh(const std::vector<Vertex>& vertices, const Shader& shader);
	
	Mesh& operator=(const Mesh& m);
	void UpdateMesh();
	void Draw(GLenum mode);
	void Cleanup();

	unsigned int VAO, VBO;
private:
	void createNewMesh();
};
#endif	// MESH_H