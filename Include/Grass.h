#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Mesh.h>

#ifndef GRASS_H
#define GRASS_H

class Grass
{
public:
	int nrInstances;
	unsigned int instanceVBO;
	std::vector<Mesh*> grassMeshes;
	std::vector<glm::mat4> modelMatrices;

	void Init(std::vector<glm::vec3> translationVectors);
	void Draw(const glm::mat4& transform);
	void Update();
};
#endif // GRASS_H