#include "Engine.h"
#include "Shader.h"
#include "Mesh.h"

#include <string>
#include <fstream>
#include <streambuf>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

std::string readTextFile(std::string filePath)
{
	std::ifstream t(filePath.c_str());
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return str;
}

int main()
{

	float triangleVertArr[] = {
	10.0f, 0.0f, 5.0f,
	5.0f, 0.0f, -5.0f,
	-5.0f, 0.0f, -5.0f
	};

	Engine engine = {};
	engine.Init();

	Shader shader("..\\Resources\\Shaders\\VertexShader.vs", "..\\Resources\\Shaders\\FragmentShader.fs");
	
	std::vector<Vertex> triangleVertices;
	std::vector<Vertex> axisVertices;
	std::vector<Vertex> planeVertices;
	std::vector<unsigned int> planeIndices{};

	//triangle
	for (int i = 0; i < 7; i+=3)
	{
		glm::vec3 pos = glm::vec3(triangleVertArr[i], triangleVertArr[i + 1], triangleVertArr[i + 2]);
		Vertex auxVertex{ pos, glm::vec3(0.0f), glm::vec3(0.0f),  glm::vec3(1.0f, 0.5f, 0.2f) };
		triangleVertices.push_back(auxVertex);
	}
	
	std::vector<unsigned int> triangleIndices{0, 1, 2};
	Mesh triangle(triangleVertices, triangleIndices);


	//axis
	glm::vec3 colors[] = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) };
	
	for (int i = 0; i < 3; i++)
	{
		Vertex auxVertex { colors[i] * 7.0f, glm::vec3(0.0f), glm::vec3(0.0f),  colors[i] };
		axisVertices.push_back(auxVertex);
		Vertex origVertex{ glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), colors[i] };
		axisVertices.push_back(origVertex);
	}

	Mesh coordSystem(axisVertices, std::vector<unsigned int>{0, 1, 2, 3, 4, 5});


	//plane
	glm::vec3 pos = glm::vec3(5.0f, 0.0f, 5.0f);

	for (int i = 1; i < 12; i++)
	{
		for (int j = 0; j < 11; j++)
		{

			Vertex auxVertex{ pos, glm::vec3(0.0f), glm::vec3(0.0f),  glm::vec3(1.0f) };
			planeVertices.push_back(auxVertex);

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

	for (int i = 0; i < planeIndices.size(); ++i)
	{
		std::cout << planeIndices[i] << ", ";
	}
	std::cout << std::endl;

	Mesh xOzPlane(planeVertices, planeIndices);
	

	glViewport(0, 0, 800, 600);

	//TODO: CHECK IF THIS NEEDS TO BE IN ENGINE::INIT()
	//register the callback function (called on every window resize)
	glfwSetFramebufferSizeCallback(engine.window, framebuffer_size_callback);
	

	//render loop
	while (!glfwWindowShouldClose(engine.window))
	{		
		//update engine time:
		engine.Update();

		//check for user input
		processInput(engine.window);

		//rendering commands
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// 4. draw the object
		shader.use();

		unsigned int transformLoc = glGetUniformLocation(shader.id, "transform");
		 
		glm::mat4 view_mat = engine.camera.getViewMatrix(); 
		glm::mat4 projection = glm::infinitePerspective(1.5f, 800.0f / 600.0f, 0.05f);
		glm::mat4 model_mat = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f));
		glm::mat4 transform = projection * view_mat * model_mat;

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		//triangle.Draw(shader, GL_TRIANGLES);

		glLineWidth(5.0);

		coordSystem.Draw(shader, GL_LINES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0);

		xOzPlane.Draw(shader, GL_TRIANGLES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// check and call events and swap the buffers
		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}
	
	engine.Stop();
	return 0;
}