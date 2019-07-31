#include <Engine.h>

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
	

	// create input manager:
	this->inputManager = InputManager(window, &mainMessageBus);

	// create world camera and subscribe to input manager:
	this->camera = Camera();
	this->mainMessageBus.addSubscriber(&camera);

	// start main bus thread:
	mainBusThread = std::thread(&MessageBus::sendMessages, &mainMessageBus);

	// start input manager thread:
	inputManagerThread = std::thread(&InputManager::listening, &inputManager);

	
	return 0;
}



int Engine::Update()
{
	// calculate time between two updated (frames)
	double currentTime = glfwGetTime();
	this->frameDelta = currentTime - this->lastFrameTime;
	this->lastFrameTime = currentTime;

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

Engine::Engine() : gameMap(100, 100) {
	this->gameMap.InitEven(2.0f);
}
