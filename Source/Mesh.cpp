#include <Mesh.h>

Vertex& Vertex::operator=(const Vertex& v)
{
	position = v.position;
	normal = v.normal;
	texCoords = v.texCoords;
	auxVars = v.auxVars;
	return *this;
}

Texture& Texture::operator=(const Texture& t)
{
	id = t.id;
	type = t.type;
	path = t.path;
	return *this;
}

Mesh::Mesh()
{
}

void Mesh::InitMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::InitMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
}

Mesh& Mesh::operator=(const Mesh& m)
{
	VAO = m.VAO;
	VBO = m.VBO;
	EBO = m.EBO;
	vertices = m.vertices;
	indices = m.indices;
	textures = m.textures;
	return *this;
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, auxVars));

	glBindVertexArray(0);

}

void Mesh::Draw(const Shader& shader, GLenum mode)
{
	glBindVertexArray(VAO);
	glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Cleanup()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
