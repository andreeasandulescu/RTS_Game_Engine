#include <Mesh.h>
#include <stb_image.h>

Vertex& Vertex::operator=(const Vertex& v)
{
	position = v.position;
	normal = v.normal;
	texCoords = v.texCoords;
	auxVars = v.auxVars;
	jointIds = v.jointIds;
	return *this;
}

Vertex::Vertex()
{
	this->position = glm::vec3(0.0f);
	this->normal = glm::vec3(0.0f);
	this->texCoords = glm::vec2(0.0f);
	this->auxVars = glm::vec4(-3.0f);
	this->jointIds = glm::uvec4(0.0f);
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, glm::vec4 auxVars)
{
	this->position = position;
	this->normal = normal;
	this->texCoords = texCoords;
	this->auxVars = auxVars;
}

Vertex::Vertex(glm::vec3 position, glm::vec4 auxVars)
{
	this->position = position;
	this->auxVars = auxVars;
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
	GLenum format;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (nrChannels == 4)
		format = GL_RGBA;
	else
		format = GL_RGB;

	if (data)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

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

void Mesh::InitMeshEBO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures, const Shader& shader)
{
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;
	this->shader = shader;

	createNewMeshEBO();
}

void Mesh::InitMeshEBO(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Shader& shader)
{
	this->vertices = vertices;
	this->indices = indices;
	this->shader = shader;

	createNewMeshEBO();
}

Mesh& Mesh::operator=(const Mesh& m)
{
	VAO = m.VAO;
	VBO = m.VBO;
	EBO = m.EBO;
	vertices = m.vertices;
	indices = m.indices;
	textures = m.textures;
	shader = m.shader;
	return *this;
}

void Mesh::createNewMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	UpdateMesh();
}

void Mesh::createNewMeshEBO()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	UpdateMeshEBO();
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
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, auxVars));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, jointIds));

	glBindVertexArray(0);
}

void Mesh::Draw(GLenum mode)
{
	char textureCName[100];

	shader.use();

	// bind textures:
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// activate texture channel i:
		sprintf_s(textureCName, "texture%d", i);
		glUniform1i(glGetUniformLocation(shader.id, textureCName), i);
	}

	
	//glUniform1i(glGetUniformLocation(shader.id, "ourTexture"), 0);

	glBindVertexArray(VAO);
	glDrawArrays(mode, 0, vertices.size());
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::Draw(const glm::mat4& transform, GLenum mode)
{
	char textureCName[100];

	shader.use();

	// bind textures:
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// activate texture channel i:
		sprintf_s(textureCName, "texture%d", i);
		glUniform1i(glGetUniformLocation(shader.id, textureCName), i);
	}

	
	//glUniform1i(glGetUniformLocation(shader.id, "ourTexture"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

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

void Mesh::UpdateMeshEBO()
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, auxVars));

	glEnableVertexAttribArray(4);
	glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, sizeof(Vertex), (void*)offsetof(Vertex, jointIds));

	glBindVertexArray(0);
}

void Mesh::DrawEBO(GLenum mode)
{
	char textureCName[100];

	shader.use();

	// bind textures:
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// activate texture channel i:
		sprintf_s(textureCName, "texture%d", i);
		glUniform1i(glGetUniformLocation(shader.id, textureCName), i);
	}

	//glUniform1i(glGetUniformLocation(shader.id, "ourTexture"), 0);

	glBindVertexArray(VAO);
	glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::DrawEBO(const glm::mat4& transform, GLenum mode)
{
	char textureCName[100];

	shader.use();

	// bind textures:
	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		// activate texture channel i:
		sprintf_s(textureCName, "texture%d", i);
		glUniform1i(glGetUniformLocation(shader.id, textureCName), i);
	}

	shader.use();
	glUniform1i(glGetUniformLocation(shader.id, "ourTexture"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(VAO);
	glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::CleanupEBO()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
