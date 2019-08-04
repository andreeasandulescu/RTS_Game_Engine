#include <water.h>

void Water::initWater(int width, int height, float altitude) {
	this->width = width;
	this->height = height;

	this->verticesMatrix.resize(height);
	for (int i = 0; i < height; i++) {
		this->verticesMatrix[i].resize(width);
		for (int j = 0; j < width; j++) {
			this->verticesMatrix[i][j].position = glm::vec3((float)i, altitude, (float)j);
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

void Water::UpdateMesh() {
	Shader shader("..\\Resources\\Shaders\\WaterVertexShader.vs", "..\\Resources\\Shaders\\WaterFragmentShader.fs");
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
