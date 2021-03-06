﻿#include <Engine.h>
#include <Shader.h>
#include <Mesh.h>
#include <Renderer.h>
#include <ResourceLoader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SceneUI.h>

bool animationIsActive = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		animationIsActive = true;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		animationIsActive = false;
	}
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

	glEnable(GL_BLEND);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 transform;

	Renderer renderer{};
	renderer.Init();

	ResourceLoader resLoader{};

	Shader shader("..\\Resources\\Shaders\\TextureVertexShader.vs", "..\\Resources\\Shaders\\TextureFragmentShader.fs");
	
	Shader newShader("..\\Resources\\Shaders\\TextureVertexShader.vs", "..\\Resources\\Shaders\\TextureFragmentShader.fs");
	AnimatedModel* animModel = resLoader.LoadAnimatedModel("..\\Resources\\Models\\walking.dae", shader);

	std::vector<Mesh*> meshes = resLoader.LoadModel("..\\Resources\\Models\\WoodenCabinObj.obj", newShader);

	glDebugMessageCallback(MessageCallback, NULL);

	glViewport(0, 0, 1360, 768);
	
	

	//TODO: CHECK IF THIS NEEDS TO BE IN ENGINE::INIT()
	//register the callback function (called on every window resize)
	glfwSetFramebufferSizeCallback(engine.window, framebuffer_size_callback);

	renderer.GenerateTriangleMesh();

	////////////////////////////////////////////////////////////////////////
	//Animated Model
	
	

	////////////////////////////////////////////////////
	
	//render loop
	while (!glfwWindowShouldClose(engine.window))
	{		
		//rendering commands
		glClearColor(0.1f, 0.0f, 0.40f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//check for user input
		processInput(engine.window);

		// draw WATER and MAP and UNITS:
		transform = engine.transform;
		engine.water.Draw(transform, engine.lightSources, engine.camera.cameraPos);
		engine.gameLogic.gameMap.Draw(transform, engine.lightSources, engine.camera.cameraPos);
		for (int i = 0; i < engine.gameLogic.playerUnits.size(); i++) {
			engine.gameLogic.playerUnits[i]->Draw(transform);
		}

		

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << err;
			std::cout << std::endl;
		}

		renderer.UpdateMatrices(engine.View, engine.Projection);
		renderer.RenderCoordSystem();

		animModel->mesh.DrawEBO(engine.Projection * engine.View, GL_TRIANGLES);

		for(int i = 0; i < meshes.size();i++)
			meshes[i]->DrawEBO(engine.Projection * engine.View, GL_TRIANGLES);


		//renderer.RenderTriangle();

		//glm::mat4 model_mat = glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)) * glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(1.0f, 0.0f, 0.0f));
		

		//update engine time:
		engine.Update();

		// check and call events and swap the buffers
		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}
	
	engine.Stop();
	return 0;
}