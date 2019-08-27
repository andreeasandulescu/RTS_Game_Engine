#include "..\Include\Grass.h"

void Grass::Init(std::vector<glm::vec3> translationVectors)
{
	glm::vec3 vertices[] = {
		// positions
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.0f),

		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f, 0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
	};

	//y is flipped because of stb image loader
	glm::vec2 texCoords[] = {
		glm::vec2(1.0f, 1.0f), // lower-right corner
		glm::vec2(1.0f, 0.0f), // upper-right corner
		glm::vec2(0.0f, 0.0f), // upper-left corner

		glm::vec2(1.0f, 1.0f), // lower-right corner
		glm::vec2(0.0f, 0.0f), // upper-left corner
		glm::vec2(0.0f, 1.0f), // lower-left corner
	};


	Shader shader = this->resourceLoader->getShader(std::string("grass"));

	Texture grassTexture = resourceLoader->getTexture("grass");
	std::vector<Texture> textVect{ grassTexture };



	float rotationAngles[3] = { 1.047f, 2.094f };

	// generate mesh for the square containing the grass texture without any rotations
	{
		std::vector<Vertex> grassPolygonVertices;

		for (int i = 0; i < 6; i++)
		{
			Vertex vert{ vertices[i], glm::vec3(0.0f), texCoords[i],  glm::vec4(0.0f) };
			grassPolygonVertices.push_back(vert);
		}

		Mesh *grassPolygon = new Mesh();
		grassPolygon->InitMeshInstanced(grassPolygonVertices, translationVectors, textVect, shader);
		grassMeshes.push_back(grassPolygon);
	}

	// generate mesh for the square containing the grass texture rotated by 60, respectively 120 degrees
	
	for(int i = 0; i < 2; i++)
	{
		std::vector<Vertex> grassPolygonVerticesRotated;
		glm::mat4 rotationM = glm::rotate(glm::mat4(1.0f), rotationAngles[i], glm::vec3(0.0f, 1.0f, 0.0f));

		for (int j = 0; j < 6; j++)
		{
			glm::vec4 newVertex = rotationM * glm::vec4(vertices[j], 1.0f);
			glm::vec3 newVertex3Comp = glm::vec3(newVertex.x, newVertex.y, newVertex.z);

			Vertex vert{ newVertex3Comp, glm::vec3(0.0f), texCoords[j],  glm::vec4(0.0f) };
			grassPolygonVerticesRotated.push_back(vert);
		}

		Mesh *grassPolygon = new Mesh();
		grassPolygon->InitMeshInstanced(grassPolygonVerticesRotated, translationVectors, textVect, shader);
		grassMeshes.push_back(grassPolygon);
	}
}


void Grass::Draw(const glm::mat4& transform)
{
	for (int i = 0; i < grassMeshes.size(); i++)
		grassMeshes[i]->DrawInstanced(transform, GL_TRIANGLES);
}

void Grass::UpdateMesh(ResourceLoader* resourceLoader) {
	this->resourceLoader = resourceLoader;
}

void Grass::Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) {

}