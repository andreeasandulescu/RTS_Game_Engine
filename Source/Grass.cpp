#include "..\Include\Grass.h"

void Grass::Init(const glm::vec3& translationVect)
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

	std::vector<Vertex> auxVertices;

	for (int i = 0; i < 6; i++)
	{
		Vertex vert{ vertices[i], glm::vec3(0.0f), texCoords[i],  glm::vec4(0.0f) };
		auxVertices.push_back(vert);
	}

	Shader shader("..\\Resources\\Shaders\\Grass\\GrassVertexShader.vs", "..\\Resources\\Shaders\\Grass\\GrassFragmentShader.fs");
	
	Texture grassTexture{};
	grassTexture.LoadTexture("..\\Resources\\Textures\\grass.png");
	std::vector<Texture> textVect{ grassTexture };

	Mesh grassPolygon;
	grassPolygon.InitMesh(auxVertices, textVect, shader);

	float rotationAngles[3] = { 0.0f, 1.047f, 2.094f };

	glm::vec3 newTranslationVect = glm::vec3(0.0f, 0.5f, 0.0f) + translationVect;		//add 0.5 to y axis to make sure the grass is above the terrain
	glm::mat4 translationM = glm::translate(glm::mat4(1.0f), newTranslationVect);

	for (int i = 0; i < 3; i++)
	{
		grassMesh.push_back(grassPolygon);
		glm::mat4 rotationM = glm::rotate(glm::mat4(1.0f), rotationAngles[i], glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 modelM = translationM * rotationM;
		modelMatrices.push_back(modelM);
	}

}

void Grass::Draw(const glm::mat4& projectionView)
{
	Texture texture = grassMesh[0].textures[0];
	
	glBindTexture(GL_TEXTURE_2D, texture.id);

	grassMesh[0].shader.use();
	glUniform1i(glGetUniformLocation(grassMesh[0].shader.id, "ourTexture"), 0);

	for (int i = 0; i < 3; i++)
	{

		glUniformMatrix4fv(glGetUniformLocation(grassMesh[0].shader.id, "modelM"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		glUniformMatrix4fv(glGetUniformLocation(grassMesh[0].shader.id, "projectionViewM"), 1, GL_FALSE, glm::value_ptr(projectionView));

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		grassMesh[i].Draw(GL_TRIANGLES);
	}

}