#include <Mesh.h>

#ifndef JOINT_H
#define JOINT_H

class Joint
{
public:
	std::vector<Joint> children;
	std::vector<Mesh> meshes;
};
#endif	// JOINT_H