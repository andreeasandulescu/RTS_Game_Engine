﻿#include <Engine.h>
#include <Shader.h>
#include <Mesh.h>
#include <Renderer.h>
#include <Animation/AnimatedModel.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

int main()
{
	Engine engine = {};
	engine.Init();

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);

	Renderer renderer{};
	renderer.Init();

	//glDebugMessageCallback(MessageCallback, NULL);

	glViewport(0, 0, 800, 600);

	//TODO: CHECK IF THIS NEEDS TO BE IN ENGINE::INIT()
	//register the callback function (called on every window resize)
	glfwSetFramebufferSizeCallback(engine.window, framebuffer_size_callback);

	renderer.GenerateTriangleMesh();

	////////////////////////////////////////////////////////////////////////
	//Animated Model
	
	AnimatedModel animModel{};
	animModel.LoadModel("..\\Resources\\Models\\cowboy.dae");

	Joint joint = animModel.rootJoint;
	Mesh myMesh = animModel.mesh;

	////////////////////////////////////////////////////
	
	//render loop
	while (!glfwWindowShouldClose(engine.window))
	{		
		//rendering commands
		glClearColor(0.2f, 0.0f, 0.20f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//check for user input
		processInput(engine.window);

		glm::mat4 view_matrix = engine.camera.getViewMatrix();
		glm::mat4 proj_matrix = glm::infinitePerspective(1.5f, 800.0f / 600.0f, 0.05f);
	
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << err;
			std::cout << std::endl;
		}

		renderer.UpdateMatrices(view_matrix, proj_matrix);
		renderer.RenderCoordSystem();

		renderer.RenderTriangle();

		

		
		
		glm::mat4 model_mat = glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)) * glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 transform = proj_matrix * view_matrix * model_mat;

		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << err;
			std::cout << std::endl;
		}

		myMesh.DrawEBO(transform, GL_TRIANGLES);

	//	animModel.jointHierarchy.children[0].meshes[0].DrawEBO(transform, GL_TRIANGLES);

		//engine.gameMap.UpdateMesh();
		//glm::mat4 transform = proj_matrix * view_matrix;
		//engine.gameMap.Draw(transform);

		// check and call events and swap the buffers

		//update engine time:
		engine.Update();

		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}
	
	engine.Stop();
	return 0;
}