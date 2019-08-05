#include <Animation/Joint.h>

void Joint::Init(int id, glm::mat4 transform)
{
	this->id = id;
	this->transform = transform;
}
