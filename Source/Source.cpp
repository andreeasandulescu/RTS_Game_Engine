#include <Engine.h>
#include <Shader.h>
#include <Mesh.h>
#include <Renderer.h>

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

	Renderer renderer{};
	renderer.Init();

	glViewport(0, 0, 800, 600);

	//TODO: CHECK IF THIS NEEDS TO BE IN ENGINE::INIT()
	//register the callback function (called on every window resize)
	glfwSetFramebufferSizeCallback(engine.window, framebuffer_size_callback);

	renderer.GenerateTriangleMesh();

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

		glm::mat4 view_mat = engine.camera.getViewMatrix(); 
		glm::mat4 projection = glm::infinitePerspective(1.5f, 800.0f / 600.0f, 0.05f);
	
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << err;
			std::cout << std::endl;
		}

		renderer.UpdateMatrices(view_mat, projection);
		renderer.RenderCoordSystem();

		renderer.RenderTriangle();

		//engine.gameMap.UpdateMesh();
		//glm::mat4 transform = projection * view_mat;
		//engine.gameMap.Draw(transform);

		// check and call events and swap the buffers
		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}
	
	engine.Stop();
	return 0;
}