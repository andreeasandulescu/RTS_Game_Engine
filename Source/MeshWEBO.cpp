#include <MeshWEBO.h>

void MeshWEBO::InitMesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const std::vector<unsigned int>& indices, const Shader& shader)
{
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;
	this->shader = shader;

	createNewMesh();
}

void MeshWEBO::InitMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader)
{
	this->vertices = vertices;
	this->indices = indices;
	this->shader = shader;

	createNewMesh();
}

Mesh& MeshWEBO::operator=(const Mesh& m)
{
	VAO = m.VAO;
	VBO = m.VBO;
	EBO = m.EBO;
	vertices = m.vertices;
	textures = m.textures;
	return *this;
}

void MeshWEBO::UpdateMesh()
{
}

void MeshWEBO::Draw(GLenum mode)
{
}

void MeshWEBO::Cleanup()
{
}

void MeshWEBO::createNewMesh()
{
}
