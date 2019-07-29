#include <Renderer.h>

Renderer::Renderer(glm::mat4 view_matrix, glm::mat4 proj_matrix)
{
	Shader shader("..\\Resources\\Shaders\\VertexShader.vs", "..\\Resources\\Shaders\\FragmentShader.fs");
	
	this->utilitiesShader = shader;
	this->view_matrix = view_matrix;
	this->proj_matrix = proj_matrix;
}

Renderer::Renderer()
{
	Shader shader("..\\Resources\\Shaders\\VertexShader.vs", "..\\Resources\\Shaders\\FragmentShader.fs");
	this->utilitiesShader = shader;
}

void Renderer::UpdateMatrices(glm::mat4 view_matrix, glm::mat4 proj_matrix)
{
	this->view_matrix = view_matrix;
	this->proj_matrix = proj_matrix;
}

void Renderer::GenerateCoordSystemMesh()
{
	std::vector<Vertex> axisVertices;
	glm::vec3 colors[] = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };

	for (int i = 0; i < 3; i++)
	{
		Vertex axisVertex{ colors[i] * 7.0f, glm::vec3(0.0f), glm::vec3(0.0f),  colors[i] };
		axisVertices.push_back(axisVertex);

		//origin vertex
		Vertex origVertex{ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), colors[i] };
		axisVertices.push_back(origVertex);
	}

	this->coordSystem.InitMesh(axisVertices, std::vector<unsigned int>{0, 1, 2, 3, 4, 5});
}

void Renderer::GeneratePlaneMesh()
{
	std::vector<Vertex> planeVertices;
	std::vector<unsigned int> planeIndices{};
	glm::vec3 pos = glm::vec3(5.0f, 0.0f, 5.0f);

	for (int i = 1; i < 12; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			Vertex vertex{ pos, glm::vec3(0.0f), glm::vec3(0.0f),  glm::vec3(1.0f) };
			planeVertices.push_back(vertex);

			pos = pos + glm::vec3(-1.0f, 0.0f, 0.0f);
		}
		pos = glm::vec3(5.0f, 0.0f, 5.0f - 1.0f * i);
	}

	int lowerPos = 0;
	int upperPos = 11;

	for (int i = 0; i < 10; i++)
	{
		lowerPos = i * 11;
		upperPos = lowerPos + 11;

		for (int j = 0; j < 10; j++)
		{
			planeIndices.push_back(lowerPos);
			planeIndices.push_back(upperPos);
			planeIndices.push_back(lowerPos + 1);

			planeIndices.push_back(upperPos);
			planeIndices.push_back(upperPos + 1);
			planeIndices.push_back(lowerPos + 1);

			lowerPos++;
			upperPos++;
		}
	}

	this->xOzPlane.InitMesh(planeVertices, planeIndices);
}


void Renderer::Init()
{
	GenerateCoordSystemMesh();
	GeneratePlaneMesh();
}

void Renderer::RenderCoordSystem()
{
	utilitiesShader.use();

	glm::mat4 model_mat = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
	glm::mat4 transform = proj_matrix * view_matrix * model_mat;

	glUniformMatrix4fv(glGetUniformLocation(utilitiesShader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(5.0);

	coordSystem.Draw(utilitiesShader, GL_LINES);
}

void Renderer::RenderXOZPlane()
{
	utilitiesShader.use();

	glm::mat4 model_mat = glm::scale(glm::mat4(1.0f), glm::vec3(12.5f));
	glm::mat4 transform = proj_matrix * view_matrix * model_mat;

	glUniformMatrix4fv(glGetUniformLocation(utilitiesShader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0);

	xOzPlane.Draw(utilitiesShader, GL_TRIANGLES);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

