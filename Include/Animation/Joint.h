#include <Mesh.h>

#include <glm/gtx/quaternion.hpp>
#include <assimp\quaternion.h>
#include <assimp/Importer.hpp>

#ifndef JOINT_H
#define JOINT_H

struct Keyframe {
	glm::vec3 translationV;
	glm::vec3 scalingV;
	aiQuaternion quatV;
	float timeInTicks;

	Keyframe();
	Keyframe(glm::vec3 translationV, glm::vec3 scalingV, aiQuaternion quatV, float timeInTicks);
	Keyframe& operator=(const Keyframe& k);
};


class Joint
{
public:
	int id;
	std::string name;			//used to identfy the aiNode/aiNodeAnim that corresponds to the the aiBone (so that we can retrieve the matrices)
	Joint* parent;
	std::vector<Joint*> children;
	std::vector<Keyframe*> keyframes;

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

	glm::quat convertToGLMQuat(const aiQuaternion& from);
	glm::mat4 getCurrentTransform(float ticksElapsed);
};
#endif	// JOINT_H