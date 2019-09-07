#include <Renderer.h>

Renderer::Renderer(const glm::mat4& view_matrix, const glm::mat4& proj_matrix)
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

void Renderer::UpdateMatrices(const glm::mat4& view_matrix, const glm::mat4& proj_matrix)
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
		Vertex axisVertex{ colors[i] * 7.0f, glm::vec4(colors[i], 1.0f) };
		axisVertices.push_back(axisVertex);

		//origin vertex
		Vertex origVertex{ glm::vec3(0.0f), glm::vec4(colors[i], 1.0f) };
		axisVertices.push_back(origVertex);
	}

	
	this->coordSystem.InitMesh(axisVertices, utilitiesShader);
}


void Renderer::Init()
{
	GenerateCoordSystemMesh();
	
}

void Renderer::RenderCoordSystem()
{

	coordSystem.shader.use();

	glm::mat4 model_mat = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));
	glm::mat4 transform = proj_matrix * view_matrix * model_mat;

	glUniformMatrix4fv(glGetUniformLocation(utilitiesShader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	coordSystem.Draw(transform, GL_LINES);
}

void Renderer::GenerateTriangleMesh()
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

	glm::vec2 texCoords[] = {
		glm::vec2(1.0f, 1.0f), // lower-right corner
		glm::vec2(1.0f, 0.0f), // upper-right corner
		glm::vec2(0.0f, 0.0f), // upper-left corner

		glm::vec2(1.0f, 1.0f), // lower-right corner
		glm::vec2(0.0f, 0.0f), // upper-left corner
		glm::vec2(0.0f, 1.0f), // lower-left corner
	};

	std::vector<Vertex> auxVertices;
	//glm::vec3 colors[] = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };

	for (int i = 0; i < 6; i++)
	{
		Vertex vert{ vertices[i], glm::vec3(0.0f), texCoords[i],  glm::vec4(0.0f) };
		auxVertices.push_back(vert);
	}

	Shader shader("..\\Resources\\Shaders\\TextureVertexShader.vs", "..\\Resources\\Shaders\\TextureFragmentShader.fs");


	Texture triangleTexture{};
	triangleTexture.LoadTexture("..\\Resources\\Textures\\grass.png");
	std::vector<Texture> textVect{ triangleTexture };

	Mesh grassPolygon;
	grassPolygon.InitMesh(auxVertices, textVect, shader);

	for (int i = 0; i < 3; i++)
	{
		grassObjMesh.push_back(grassPolygon);
	}
	
}

void Renderer::RenderTriangle()
{
	Texture texture = grassObjMesh[0].textures[0];
	float rotationAngles[3] = { 0.0f, 1.047f, 2.094f };
	glm::mat4 scalingMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 rotationMat;

	glBindTexture(GL_TEXTURE_2D, texture.id );
	
	grassObjMesh[0].shader.use();

	glUniform1i(glGetUniformLocation(grassObjMesh[0].shader.id, "ourTexture"), 0);
	

	for (int i = 0; i < 3; i++)
	{
		rotationMat = glm::rotate(glm::mat4(1.0f), rotationAngles[i], glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f));

		glm::mat4 model_mat = scalingMat * rotationMat * translationMat;
		glm::mat4 transform = proj_matrix * view_matrix * model_mat;

		glUniformMatrix4fv(glGetUniformLocation(grassObjMesh[0].shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		grassObjMesh[i].Draw(transform, GL_TRIANGLES);
	}


}