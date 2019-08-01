#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include<Shader.h>
#include<Mesh.h>

#ifndef DRAWABLE_H
#define DRAWABLE_H

class Drawable {
public:
	Mesh mesh;
	virtual void UpdateMesh() = 0;
	virtual void Draw(const glm::mat4& transform) = 0;
};


#endif	// DRAWABLE_H