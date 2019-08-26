#include <Animation/AnimatedModel.h>
#include "..\..\Include\Camera.h"


void AnimatedModel::Cleanup()
{
	//TODO!!!!
}

void AnimatedModel::generateGlobalAnimationMatrices(Joint *joint)
{
	//the root joint's global animation matrix is its local animation matrix
	if(joint->parent == nullptr)
		joint->setGlobalAnimationM(joint->localAnimationM);
	else
	{
		glm::mat4 globalAnimationM = joint->parent->globalAnimationM * joint->localAnimationM;
		joint->setGlobalAnimationM(globalAnimationM);

	}
		
	unsigned int childCnt = joint->children.size();
	for (unsigned int i = 0; i < childCnt; i++)
	{
		generateGlobalAnimationMatrices(joint->children[i]);
	}
}

void AnimatedModel::setFinalJointTransforms()
{
	for (unsigned int i = 0; i < joints.size(); i++)
	{
		glm::mat4 invBindPoseM = joints[i]->invBindPoseM;
		glm::mat4 globalAnimationM = joints[i]->globalAnimationM;
		glm::mat4 finalM = globalInverseTransform * globalAnimationM * glm::transpose(invBindPoseM);
		
		jointTransforms[i] = finalM;
	}
}

void AnimatedModel::runAnimation()
{
	float timeElapsed = (float)glfwGetTime() - startTime;
	float ticksElapsed = fmod((timeElapsed * ticksPerSecond), durationInTicks);
	
	for (unsigned int i = 0; i < joints.size(); i++)
	{
		glm::mat4 modelM = joints[i]->getCurrentTransform(ticksElapsed);
		joints[i]->setLocalAnimationM(modelM);
	}

	generateGlobalAnimationMatrices(rootJoint);


	setFinalJointTransforms();

}

void AnimatedModel::Draw(bool animationActive, glm::mat4 transform)
{
	jointTransforms.resize(joints.size());
	if (this->startTime < 0.0f && !animationActive)
	{
		std::cout << "start time set!!!\n";
		startTime = (float)glfwGetTime();
	}

	if (!animationActive)
	{
		for (unsigned int i = 0; i < joints.size(); i++)
			jointTransforms[i] = globalInverseTransform;
	}
	else
		runAnimation();

	glm::mat4* ptr = jointTransforms.data();
	mesh.shader.use();

	glUniformMatrix4fv(glGetUniformLocation(mesh.shader.id, "jointTransforms"), 20, GL_FALSE, reinterpret_cast<GLfloat*>(&ptr[0]));

	mesh.DrawEBO(transform, GL_TRIANGLES);
}