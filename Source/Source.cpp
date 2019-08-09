#include <Engine.h>
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
	
	glm::mat4 transform;

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

	

	////////////////////////////////////////////////////
	
	//render loop
	while (!glfwWindowShouldClose(engine.window))
	{		
		//rendering commands
		glClearColor(0.2f, 0.0f, 0.20f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//check for user input
		processInput(engine.window);

		// draw WATER and MAP and UNIT:
		transform = engine.transform;
		engine.water.Draw(transform, engine.lightSources, engine.camera.cameraPos);
		engine.gameMap.Draw(transform, engine.lightSources, engine.camera.cameraPos);
		engine.testUnit.Draw(transform);

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << err;
			std::cout << std::endl;
		}

		renderer.UpdateMatrices(engine.Projection, engine.View);
		renderer.RenderCoordSystem();

		renderer.RenderTriangle();

		

		
		
		//glm::mat4 model_mat = glm::scale(glm::mat4(1.0f), glm::vec3(0.35f)) * glm::rotate(glm::mat4(1.0f), -1.57f, glm::vec3(1.0f, 0.0f, 0.0f));
		

		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << err;
			std::cout << std::endl;
		}


		glm::mat4* ptr = animModel.jointTransforms.data();
		animModel.mesh.shader.use();
		glUniformMatrix4fv(glGetUniformLocation(animModel.mesh.shader.id, "jointTransforms"), 20, GL_FALSE, reinterpret_cast<GLfloat*>(&ptr[0]));

		animModel.mesh.DrawEBO(transform, GL_TRIANGLES);

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