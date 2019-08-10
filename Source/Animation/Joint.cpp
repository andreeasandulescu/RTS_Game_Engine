#include <Animation/Joint.h>

//Keyframe
Keyframe::Keyframe()
{
}

Keyframe::Keyframe(glm::vec3 translationV, glm::vec3 scalingV, aiQuaternion quatV, float timeInTicks)
{
	this->translationV = translationV;
	this->scalingV = scalingV;
	this->quatV = quatV;
	this->timeInTicks = timeInTicks;
}

Keyframe& Keyframe::operator=(const Keyframe& k)
{
	translationV = k.translationV;
	scalingV = k.scalingV;
	quatV = k.quatV;
	timeInTicks = k.timeInTicks;
	return *this;
}

//Joint
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

glm::quat Joint::convertToGLMQuat(const aiQuaternion& from)
{
	glm::quat to;
	to.x = from.x;
	to.y = from.y;
	to.z = from.z;
	to.w = from.w;
	return to;
}

glm::mat4 Joint::getCurrentTransform(float ticksElapsed)
{
	unsigned int index = 0;

	for (index = 0; index < keyframes.size() - 1 ; index++)
	{
		if (ticksElapsed <= keyframes[index + 1]->timeInTicks)
		{
			break;
		}
	}

	Keyframe *currFrame, *nextFrame;
	currFrame = keyframes[index];
	nextFrame = keyframes[fmod(index + 1, keyframes.size())];

	float deltaTime = abs(nextFrame->timeInTicks - currFrame->timeInTicks);
	
	float factor = (ticksElapsed - currFrame->timeInTicks) / deltaTime;		//between 0.0 and 1.0

	if (factor < 0.0f || factor > 1.0f)
		std::cout << "Error! Factor should be in [0.0, 1.0]\n";

	//translation
	glm::vec3 deltaT = nextFrame->translationV - currFrame->translationV;
	glm::vec3 newTranslationV = currFrame->translationV + factor * deltaT;
	glm::mat4 translationM = glm::translate(glm::mat4(1.0f), newTranslationV);

	//rotation
	aiQuaternion newQuat;
	const aiQuaternion& currentQuat = currFrame -> quatV;
	const aiQuaternion& nextQuat = nextFrame -> quatV;
	aiQuaternion::Interpolate(newQuat, currentQuat, nextQuat, factor);

	glm::quat newGLMQuat = convertToGLMQuat(newQuat);
	glm::mat4 rotationM = glm::toMat4(newGLMQuat);

	//scaling
	glm::vec3 deltaS = nextFrame->scalingV - currFrame->scalingV;
	glm::vec3 newScalingV = currFrame->scalingV + factor * deltaS;
	glm::mat4 scalingM = glm::scale(glm::mat4(1.0f), newScalingV);


	return translationM * rotationM * scalingM;
}
