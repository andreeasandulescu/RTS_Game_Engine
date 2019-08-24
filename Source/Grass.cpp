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


	Shader shader("..\\Resources\\Shaders\\Grass\\GrassVertexShader.vs", "..\\Resources\\Shaders\\Grass\\GrassFragmentShader.fs");

	Texture grassTexture{};
	grassTexture.LoadTexture("..\\Resources\\Textures\\grass.png");
	std::vector<Texture> textVect{ grassTexture };

	std::vector<Vertex> grassPolygonVertices;


	float rotationAngles[3] = { 1.047f, 2.094f };

	// generate mesh for the square containing the grass texture without any rotations
	{
		for (int i = 0; i < 6; i++)
		{
			Vertex vert{ vertices[i], glm::vec3(0.0f), texCoords[i],  glm::vec4(0.0f) };
			grassPolygonVertices.push_back(vert);
		}

		Mesh *grassPolygon = new Mesh();
		grassPolygon->InitMesh(grassPolygonVertices, textVect, shader);
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
		grassPolygon->InitMesh(grassPolygonVerticesRotated, textVect, shader);
		grassMeshes.push_back(grassPolygon);
	}

	//create VBO for translation vectors (instanced array)
	unsigned int translationVectorsCnt = translationVectors.size();
	nrInstances = translationVectorsCnt;
		
	glGenBuffers(1, &this->instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, translationVectorsCnt * sizeof(glm::vec3), &translationVectors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Update();
}



void Grass::Draw(const glm::mat4& transform)
{
	glDisable(GL_CULL_FACE);
	/*
	

	grassMeshes[0]->shader.use();


	for (int i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glUniform1i(glGetUniformLocation(grassMeshes[0]->shader.id, "ourTexture"), 0);

		glBindVertexArray(grassMeshes[i]->VAO);
		
		glDrawArrays(GL_TRIANGLES, 0, grassMeshes[i]->vertices.size());
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}

	

	// always good practice to set everything back to defaults once configured.

	//////////////
	for (int i = 0; i < 3; i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(grassMeshes[0].shader.id, "modelM"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		glUniformMatrix4fv(glGetUniformLocation(grassMeshes[0].shader.id, "projectionViewM"), 1, GL_FALSE, glm::value_ptr(projectionView));

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		grassMeshes[i].Draw(GL_TRIANGLES);
	}
	*/
	Texture texture = grassMeshes[0]->textures[0];

	grassMeshes[0]->shader.use();


	for (int i = 0; i < 3; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glUniformMatrix4fv(glGetUniformLocation(grassMeshes[0]->shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

		
		glBindVertexArray(grassMeshes[i]->VAO);
		
		glDrawArraysInstanced(GL_TRIANGLES, 0, grassMeshes[i]->vertices.size(), nrInstances);

		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}
}

void Grass::Update()
{
	for (int i = 0; i < 3; i++)
	{
		Mesh *polygonMesh = grassMeshes[i];
		glBindVertexArray(polygonMesh->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, polygonMesh->VBO);
		glBufferData(GL_ARRAY_BUFFER, polygonMesh->vertices.size() * sizeof(Vertex), &polygonMesh->vertices[0], GL_STREAM_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		//instanced data
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}