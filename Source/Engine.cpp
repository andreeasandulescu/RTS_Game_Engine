#include "Engine.h"

int Engine::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Coolest RTS Game Engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// cursor does not leave window:
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	this->window = window;

	///// !!!!!! In order for any OpenGL commands to work, a context must be current  !!!!!!!
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// create world camera:
	this->camera = Camera();

	return 0;
}

int Engine::Update()
{
	// calculate time between two updated (frames)
	double currentTime = glfwGetTime();
	this->frameDelta = currentTime - this->lastFrameTime;
	this->lastFrameTime = currentTime;

	// update camera:
	double xpos;
	double ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	this->camera.mouse_callback(window, xpos, ypos);
	return 0;
}



float Engine::getFrameDelta()
{
	return this->frameDelta;
}

int Engine::Stop()
{
	glfwTerminate();
	return 0;
}