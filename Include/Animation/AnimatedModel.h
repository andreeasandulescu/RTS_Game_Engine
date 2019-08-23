#include <map>
#include <utility>
#include <math.h>
#include <algorithm>

#include <Mesh.h>
#include <Animation/Joint.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

class AnimatedModel
{
public:
	Joint *rootJoint;
	Mesh mesh;

	std::vector<Joint*> joints;
	std::vector<glm::mat4> jointTransforms;
	std::map<std::string, unsigned int> names;

	float startTime = -1.0f;
	float durationInTicks;
	float ticksPerSecond;

	glm::mat4 globalInverseTransform;

	void LoadModel(std::string path);
	void runAnimation();					//time elapsed since the first command to run the animation (seconds)
	void Draw(bool animationActive, glm::mat4 transform);
	void Cleanup();				//TODO!!!!


private:
	void generateGlobalAnimationMatrices(Joint* joint);
	void setFinalJointTransforms();

};
#endif	// ANIMATEDMODEL_H