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

	if (currentUnitCommand.cmdType == CommandType::STOP) {
		speed = 0.0f;
	}
	else if (currentUnitCommand.cmdType == CommandType::MOVE) {
		speed = maxSpeed;
		// check if unit arrived at the position:
		if (glm::distance(position, currentUnitCommand.positon) < 1.0f) {
			currentUnitCommand.cmdType = CommandType::STOP;
		}
		
	} 


	position = position + (deltaFrame * speed) * direction;
}

void Unit::UpdateMesh() {

}

void Unit::Draw(const glm::mat4& transform) {
	bool animationActive = true;
	glm::mat4 rotationM = glm::mat4(1);
	glm::mat4 scaleM = glm::mat4(1);

	if (currentUnitCommand.cmdType == CommandType::STOP) {
		animationActive = false;
	}
	else {
		// compute unit direction:
		direction = normalize(currentUnitCommand.positon - position);

	}
	
	// rotate model to direction:
	float cosAngle = glm::dot(direction, glm::vec3(0.0f, 0.0f, 1.0f));
	float angle = acos(cosAngle);
	if (direction.x < 0) {
		angle = -angle;
	}
	
	rotationM = glm::rotate(glm::mat4(1), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	//rotationM = glm::lookAt(position, 2.0f * direction + position, glm::vec3(0.0f, 1.0f, 0.0f));
	

	// scale model:
	float scale = 0.2f;
	scaleM = glm::scale(glm::mat4(1), glm::vec3(scale));

	glm::mat4 finalTransform = transform * glm::translate(glm::mat4(1.0), position) * scaleM * rotationM;

	newAnimModel->Draw(
		animationActive,
		finalTransform
	);
}

void Unit::Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) {
	
}

Unit::Unit() {
	speed = 0.0f;
	selectionRadius = 2.0f;
	direction = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
	name = std::string("Unnamed_unit");
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	currentUnitCommand = UnitCommand();
	maxSpeed = 6.0f;
}