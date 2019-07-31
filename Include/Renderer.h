#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Mesh.h>

#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
public:
	Shader utilitiesShader;
	Mesh coordSystem;
	Mesh xOzPlane;
	glm::mat4 view_matrix;
	glm::mat4 proj_matrix;
	
	Renderer(glm::mat4 view_matrix, glm::mat4 proj_matrix);
	Renderer();
	void UpdateMatrices(glm::mat4 view_matrix, glm::mat4 proj_matrix);
	void Init();
	void RenderCoordSystem();
	void RenderXOZPlane();
	void RenderMapTile(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);

private:
	void GenerateCoordSystemMesh();
	void GeneratePlaneMesh();
	Mesh GenerateMapTile(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);

};
#endif // RENDERER_H