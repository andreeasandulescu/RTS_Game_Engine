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

int main()
{
	Engine engine = {};
	engine.Init();

	Renderer renderer{};
	renderer.Init();

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

		renderer.RenderXOZPlane();

		glm::vec3 pos = glm::vec3(2.0f, 2.0f, 2.0f);
		renderer.RenderMapTile(pos, glm::vec3(pos.x + 2, pos.y, pos.z), glm::vec3(pos.x + 2, pos.y, pos.z + 2), glm::vec3(pos.x , pos.y, pos.z + 2));

		// check and call events and swap the buffers
		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}
	
	engine.Stop();
	return 0;
}