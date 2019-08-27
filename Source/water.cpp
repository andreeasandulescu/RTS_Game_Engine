#include <water.h>

void Water::initWater(int width, int height, float scale, float altitude) {
	width = width / scale;
	height = height / scale;
	
	this->width = width;
	this->height = height;

	this->verticesMatrix.resize(height);
	for (int i = 0; i < height; i++) {
		this->verticesMatrix[i].resize(width);
		for (int j = 0; j < width; j++) {
			this->verticesMatrix[i][j].position = glm::vec3((float)i * scale, altitude, (float)j * scale);
		}
	}
	/*
	                     +
       +             |
i,j+1  |             | i+1,j+1
       +---------+-----------+
       |         - --+
       | 1tri  --    |
       |     --      |
       |  --     2tri|
i,j    | -           | i,j+1
  +--+ +-+-----------+
       |             |
       |             |
       +             +

	*/

	// specifiy the indices for uniting the vertices:
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			
			// first square:
			this->indices.push_back(i * width + j);
			this->indices.push_back((i + 1) * width + j + 1);
			this->indices.push_back((i + 1) * width + j);

			// second square:
			this->indices.push_back(i * width + j);
			this->indices.push_back(i * width + j + 1);
			this->indices.push_back((i + 1) * width + j + 1);
		}
	}
	
}

void Water::UpdateMesh(ResourceLoader* resourceLoader) {
	this->resourceLoader = resourceLoader;

	Shader shader = resourceLoader->getShader(std::string("water"));

	std::vector<Vertex> EBOvertices;
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			EBOvertices.push_back(verticesMatrix[i][j]);
		}
	}

	mesh.InitMeshEBO(EBOvertices, indices, shader);
}

void Water::Draw(const glm::mat4& transform) {
	mesh.shader.use();

	// send time:
	float time = glfwGetTime();
	GLint loc = glGetUniformLocation(mesh.shader.id, "time");
	if (loc != -1)
	{
		glUniform1f(loc, time);
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glLineWidth(1.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	mesh.DrawEBO(transform, GL_TRIANGLES);
}

void Water::Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) {
	char loc_name[100];

	// translate water to origin:
	glm::vec3 translation = glm::vec3(width / -3.0f, 0.0f, height / -3.0f);
	glm::mat4 translationM = glm::translate(glm::mat4(1), translation);

	mesh.shader.use();

	// send time:
	float time = glfwGetTime();
	GLint loc = glGetUniformLocation(mesh.shader.id, "time");
	if (loc != -1) {
		glUniform1f(loc, time);
	}

	// send camera position:
	loc = glGetUniformLocation(mesh.shader.id, "viewerPos");
	glUniform3fv(loc, 1, value_ptr(cameraPos));

	// send number light sources:
	loc = glGetUniformLocation(mesh.shader.id, "nrLights");
	glUniform1i(loc, lightSources.size());

	// send lighting data:
	for (int i = 0; i < lightSources.size(); i++) {
		sprintf_s(loc_name, "Light[%d].Position", i);
		loc = glGetUniformLocation(mesh.shader.id, loc_name);
		glUniform3fv(loc, 1, value_ptr(lightSources[i]->sourcePosition));

		sprintf_s(loc_name, "Light[%d].Color", i);
		loc = glGetUniformLocation(mesh.shader.id, loc_name);
		glUniform3fv(loc, 1, value_ptr(lightSources[i]->color));
	}
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	mesh.DrawEBO(transform * translationM, GL_TRIANGLES);
}
