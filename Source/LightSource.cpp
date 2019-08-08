#include <LightSource.h>

LightSource::LightSource() {
	this->sourcePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->color = glm::vec3(0.5f, 0.3f, 0);
	this->intensity = 1.0f;
}