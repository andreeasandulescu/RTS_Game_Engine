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
#include <stb_image.h>

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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	unsigned int textureID;
	//glGenTextures(1, &textureID);
	//glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	////////ADD DEBUG CALLBACK///////////////
	// During init, enable debug output
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	int width, height, nrChannels;
	unsigned char* data = stbi_load("..\\Resources\\Textures\\container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		//glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	
	//stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

		renderer.auxMesh.shader.use();
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glUniform1i(glGetUniformLocation(renderer.auxMesh.shader.id, "ourTexture"), 0);


		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << err;
			std::cout << std::endl;
		}

		renderer.RenderTriangle();

		//engine.gameMap.UpdateMesh();
		//glm::mat4 transform = projection * view_mat;
		//engine.gameMap.Draw(transform);

		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cerr << err;
			std::cout << std::endl;
		}

		// check and call events and swap the buffers
		glfwSwapBuffers(engine.window);
		glfwPollEvents();
	}
	
	engine.Stop();
	return 0;
}