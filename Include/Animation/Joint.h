#include <Mesh.h>

#ifndef JOINT_H
#define JOINT_H

class Joint
{
public:
	int id;
	glm::mat4 transform;
	std::vector<Joint> children;
	
	void Init(int id, glm::mat4 transform);

};
#endif	// JOINT_H