#include <Mesh.h>

#ifndef JOINT_H
#define JOINT_H

class Joint
{
public:
	int id;
	Joint* parent;
	std::vector<Joint*> children;
	std::string name;			//used only to identfy the aiNode that corresponds to the the aiBone (so that we can retrieve the matrices)

	//matrices
	glm::mat4 invBindPoseM;		//inverse bind pose matrix -> default pose of skeleton, prior to applying any animation
	glm::mat4 localAnimationM;		//local pose matrix -> relative to a parent bone
	glm::mat4 globalAnimationM;		//global pose matrix -> relative to the object space origin
	
	Joint();
	Joint& operator=(const Joint& j);

	void setId(int id);
	void setName(std::string name);
	void setParent(Joint *parent);
	void setChildren(const std::vector<Joint*>& children);
	void setInvBindPoseM(glm::mat4 invBindPoseM);
	void setLocalAnimationM(glm::mat4 localAnimationM);
	void setGlobalAnimationM(glm::mat4 globalAnimationM);
};
#endif	// JOINT_H