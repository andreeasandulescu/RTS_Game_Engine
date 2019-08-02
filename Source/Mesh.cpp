#include <Mesh.h>
#include <stb_image.h>

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

void Texture::LoadTexture(const GLchar* texturePath)
{
	unsigned int id;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		this->id = id;
		this->path = texturePath;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

}

Mesh::Mesh()
{
}

void Mesh::InitMesh(const std::vector<Vertex>& vertices, const std::vector<Texture>& textures, const Shader& shader)
{
	this->vertices = vertices;
	this->textures = textures;
	this->shader = shader;

	createNewMesh();
}

void Mesh::InitMesh(const std::vector<Vertex>& vertices, const Shader& shader)
{
	this->vertices = vertices;
	this->shader = shader;

	createNewMesh();
}

Mesh& Mesh::operator=(const Mesh& m)
{
	VAO = m.VAO;
	VBO = m.VBO;
	vertices = m.vertices;
	textures = m.textures;
	return *this;
}

void Mesh::createNewMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	UpdateMesh();
}

void Mesh::UpdateMesh()
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STREAM_DRAW);

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

void Mesh::Draw(GLenum mode)
{
	glBindVertexArray(VAO);
	glDrawArrays(mode, 0, vertices.size());
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Cleanup()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
