#include <Drawable.h>
#include <UnitCommand.h>

#ifndef UNIT_H
#define UNIT_H

/*
	Basic unit in the game that can move
	with a given speed and has a position.
	It can also be rendered.
*/

class Unit : public Drawable {
public:
	std::string name;
	glm::vec3 position;
	glm::vec3 direction;
	AnimatedModel* newAnimModel;

	// current unit movement speed
	float speed;

	// max movement speed
	float maxSpeed;

	// select the unit within a spherical radius:
	float selectionRadius;
	Mesh mesh;

	// unit always has a current command
	// default current command is STOP:
	UnitCommand currentUnitCommand;

	// update position by speed and direction:
	virtual void updateUnit(float deltaFrame);

	// Functions bellow should only be called in an OpenGL context:
	// Creates a Mesh for the unit
	virtual void initUnit();

	// Updates unit Mesh/Animation
	virtual void UpdateMesh();
	
	virtual void Draw(const glm::mat4& transform);
	virtual void Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos);

	Unit();
};

#endif // UNIT_H