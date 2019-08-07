#include <Animation/Joint.h>

Joint::Joint()
{
	this->parent = nullptr;
}

Joint& Joint::operator=(const Joint& j)
{
	this->id = j.id;
	this->parent = j.parent;
	this->children = j.children;
	this->invBindPoseM = j.invBindPoseM;
	this->localAnimationM = j.localAnimationM;
	this->globalAnimationM = j.globalAnimationM;
	this->name = j.name;

	return *this;
}

void Joint::setId(int id)
{
	this->id = id;
}

void Joint::setName(std::string name)
{
	this->name = name;
}

void Joint::setParent(Joint* parent)
{
	this->parent = parent;
}

void Joint::setChildren(const std::vector<Joint*>& children)
{
	this->children = children;
}

void Joint::setInvBindPoseM(glm::mat4 invBindPoseM)
{
	this->invBindPoseM = invBindPoseM;
}

void Joint::setLocalAnimationM(glm::mat4 localAnimationM)
{
	this->localAnimationM = localAnimationM;
}

void Joint::setGlobalAnimationM(glm::mat4 globalAnimationM)
{
	this->globalAnimationM = globalAnimationM;
}
