#include <Building.h>

void Building::Draw(const glm::mat4& transform) {
	
	glm::mat4 finalTransform = transform * glm::translate(glm::mat4(1), position) * rotation;
	mesh.DrawEBO(finalTransform, GL_TRIANGLES);
}

Building::Building() {
	speed = 0.0f;
	selectionRadius = 10.0f;
	direction = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
	name = std::string("Unnamed_building");
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	currentUnitCommand = UnitCommand();
	rotation = glm::mat4(1.0f);
	maxSpeed = 0.0f;
}

