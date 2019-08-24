#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.h>
#include <Mesh.h>
#include <LightSource.h>
#include <ResourceLoader.h>

#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable {
public:
	Mesh mesh;

	// implement this to update the mesh above
	// update only when necessary
	virtual void UpdateMesh() = 0;
	
	// draw every frame mesh
	virtual void Draw(const glm::mat4& transform) = 0;
	virtual void Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) = 0;
};


#endif	// DRAWABLE_H