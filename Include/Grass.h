#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Mesh.h>
#include <Drawable.h>

#ifndef GRASS_H
#define GRASS_H

class Grass : public Drawable
{
public:
	
	std::vector<Mesh*> grassMeshes;

	void Init(std::vector<glm::vec3> translationVectors);
	virtual void Draw(const glm::mat4& transform);
	virtual void UpdateMesh(ResourceLoader* resourceLoader);

	virtual void Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos);

};
#endif // GRASS_H