#include <Unit.h>

void Unit::initUnit() {
	// Create a cubic mesh for the  Unit:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// load a shader for the unit:
	Shader shader("..\\Resources\\Shaders\\VertexShader.vs", "..\\Resources\\Shaders\\FragmentShader.fs");

	// Define cube positions:
	vertices.resize(8);

	vertices[0].position = glm::vec3(0, 0, 0);
	vertices[1].position = glm::vec3(0, 0, 1);
	vertices[2].position = glm::vec3(1, 0, 1);
	vertices[3].position = glm::vec3(1, 0, 0);
	vertices[4].position = glm::vec3(0, 1, 0);
	vertices[5].position = glm::vec3(0, 1, 1);
	vertices[6].position = glm::vec3(1, 1, 1);
	vertices[7].position = glm::vec3(1, 1, 0);

	// v1v2v3
	indices = std::vector<unsigned int>
	{
		0, 1, 2,
		0, 2, 3,
		0, 1, 5,
		0, 5, 4,
		1, 2, 6,
		1, 6, 5,
		3, 2, 6,
		3, 6, 7,
		0, 3, 7,
		0, 7, 4,
		5, 6, 4,
		4, 6, 7
	};

	mesh.InitMeshEBO(vertices, indices, shader);
}

void Unit::updateUnit(float deltaFrame) {
	position = position + (deltaFrame * speed) * direction;
}

void Unit::UpdateMesh() {

}

void Unit::Draw(const glm::mat4& transform) {
	glm::mat4 mvp = transform * glm::translate(glm::mat4(1), position);
	mesh.DrawEBO(mvp, GL_TRIANGLES);
}

void Unit::Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) {
	
}

Unit::Unit() {
	this->speed = 0;
	this->direction = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));
	this->name = std::string("Unnamed_unit");
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
}