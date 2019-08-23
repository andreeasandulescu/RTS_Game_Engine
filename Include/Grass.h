#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Mesh.h>

#ifndef GRASS_H
#define GRASS_H

class Grass
{
public:
	std::vector<Mesh> grassMesh;
	std::vector<glm::mat4> modelMatrices;

	void Init(const glm::vec3& translationVect);
	void Draw(const glm::mat4& projectionView);
};
#endif // GRASS_H