#include <Unit.h>

#ifndef BUILDING_H
#define BUILDING_H

class Building : public Unit {
public:
	glm::mat4 rotation;

	virtual void Draw(const glm::mat4& transform);
	Building();
};

#endif // BUILDING_H