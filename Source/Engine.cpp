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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// dont change mouse button state until polling:
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

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

	// add engine to the main message bus:
	this->mainMessageBus.addSubscriber(this);

	// create GUI:
	gui = GUI::GUI();
	gui.initGUI();

	// start main bus thread:
	mainBusThread = std::thread(&MessageBus::sendMessages, &mainMessageBus);

	// start input manager thread:
	inputManagerThread = std::thread(&InputManager::listening, &inputManager);

	// load game map:
	int width, height, nrChannels;
	unsigned char* data = stbi_load("..\\Resources\\Textures\\terrain_height.jpg", &width, &height, &nrChannels, 0);
	gameMap.InitEven(0.0f);
	gameMap.loadHeightMap(data, nrChannels, width, height);
	// update normals:
	gameMap.smoothNormals();
	gameMap.UpdateMesh();
	
	// load water map:
	water.initWater(gameMap.width, gameMap.height, 0.7f, 2);
	water.UpdateMesh();

	// load lighting:
	this->lightSources = std::vector<LightSource *>();
	LightSource* sun = new LightSource();
	sun->sourcePosition = glm::vec3(1000, 1000, 1000);
	sun->intensity = 1.0f;
	sun->color = glm::vec3(1.0f, 1.0f, 1.0f);
	lightSources.push_back(sun);

	// load unit:
	testUnit.initUnit();
	testUnit.position = glm::vec3(10, 10, 10);
	testUnit.speed = 0.5f;

	return 0;
}

void Engine::receiveMessage(Message* m) {
	if (m->messageType == MessageType::cursor) {
		CursorMessage* c = (CursorMessage*)m;
		if (c->cursorState.left == ButtonStatus::PRESSED) {
			printf("Left click pressed at: (%d, %d).", c->cursorState.xPos, c->cursorState.yPos);
		}
	}
	delete m;
}

int Engine::Update()
{
	// update proj and view matricies:
	View = camera.getViewMatrix();
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	Projection = glm::infinitePerspective(1.5f, (float)width / (float)height, 0.05f);
	transform = Projection * View;

	// calculate time between two updated (frames)
	double currentTime = glfwGetTime();
	this->frameDelta = currentTime - this->lastFrameTime;
	this->lastFrameTime = currentTime;
	this->gui.guiUpdate(this->window);
	
	// update units positions:
	testUnit.updateUnit(frameDelta);


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

Engine::Engine() : gameMap(200, 200) {
	this->gameMap.InitEven(2.0f);
	this->transform = glm::mat4(1);
}
