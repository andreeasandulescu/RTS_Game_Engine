#include <glm/gtc/type_ptr.hpp>

#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

class LightSource {
public:
	glm::vec3 sourcePosition;
	glm::vec3 color;
	float intensity;

	LightSource();
};

#endif // LIGHT_SOURCE_H