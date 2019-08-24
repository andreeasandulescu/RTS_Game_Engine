#include <Engine.h>

int Engine::Init()
{
	int width, height, nrChannels;

	width = 1360;
	height = 768;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Coolest RTS Game Engine", NULL, NULL);
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

	// allocate buffer space:
	glfwGetWindowSize(window, &width, &height);
	this->windowWidth = width;
	this->windowHeight = height;
	depthBuffer = new float[(size_t)width * height];

	///// !!!!!! In order for any OpenGL commands to work, a context must be current  !!!!!!!
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	// black screen:
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glfwSwapBuffers(window);

	glEnable(GL_DEPTH_TEST);
	// create input manager:
	this->inputManager = InputManager(window, &mainMessageBus);

	// create world camera and subscribe to input manager:
	this->camera = Camera();
	this->mainMessageBus.addSubscriber(&camera);

	// create resource loader:
	resLoader = new ResourceLoader();

	// add engine to the main message bus:
	this->mainMessageBus.addSubscriber(this);

	// create GUI:
	gui = GUI::GUI();
	gui.initGUI();

	// create user interface:
	userInterface.initUserInterface(window, &gui, &mainMessageBus);
	userInterface.UpdateMesh();
	// add user interface to message receiver bus:
	mainMessageBus.addSubscriber(&userInterface);

	// load game logic
	gameLogic.resLoader = resLoader;
	gameLogic.initGameLogic();
	

	// add game logic to the main message bus:
	gameLogic.messageBus = &mainMessageBus;
	this->mainMessageBus.addSubscriber(&gameLogic);

	// start main bus thread:
	mainBusThread = std::thread(&MessageBus::sendMessages, &mainMessageBus);

	// start input manager thread:
	inputManagerThread = std::thread(&InputManager::listening, &inputManager);

	
	
	// load water map:
	water.initWater(gameLogic.gameMap.width, gameLogic.gameMap.height, 0.7f, 2);
	water.UpdateMesh();

	// load lighting:
	this->lightSources = std::vector<LightSource *>();
	LightSource* sun = new LightSource();
	sun->sourcePosition = glm::vec3(1000, 1000, 1000);
	sun->intensity = 1.0f;
	sun->color = glm::vec3(1.0f, 1.0f, 1.0f);
	lightSources.push_back(sun);

	// load unit:
	for (int i = 0; i < gameLogic.playerUnits.size(); i++) {
		gameLogic.playerUnits[i]->initUnit();
	}

	return 0;
}

void Engine::receiveMessage(Message* m) {
	if (m->messageType == MessageType::cursor) {
		CursorMessage* c = (CursorMessage*)m;

		// determine clicked position in real world coordinates:
		glm::vec4 posAux = glm::vec4(1);
		glm::vec4 pos = glm::vec4(1);

		

		if (c->cursorState.left == ButtonStatus::PRESSED || c->cursorState.right == ButtonStatus::PRESSED) {

			if (c->cursorState.xPos > 0 && c->cursorState.yPos > 0) {
				posAux.x = (2.0f * ((float)(c->cursorState.xPos) / windowWidth)) - 1.0f,
				posAux.y = 1.0f - (2.0f * ((float)c->cursorState.yPos / windowHeight));
				posAux.z = 2.0 * depthBuffer[(windowHeight - (int)c->cursorState.yPos) * windowWidth + (int)c->cursorState.xPos] - 1.0;
				posAux.w = 1.0f;

				pos = (glm::inverse(transform) * posAux);
				pos = pos / pos.w;
			

				// send back a message:
				WorldClick* wClick = new WorldClick();
				wClick->cursorState = c->cursorState;
				wClick->worldPosition = pos;
				mainMessageBus.addMessage(wClick);

			}
		}
	}

	if (m->messageType == MessageType::buttonpress) {
		ButtonPressed* buttonMsg = (ButtonPressed*)m;
		printf("Button pressed: %s\n", buttonMsg->messageString.c_str());

		if (buttonMsg->action == ButtonAction::exitAction) {
			exit(0);
		}

		if (buttonMsg->action == ButtonAction::playAction) {
			this->userInterface.currentScene = NULL;
		}
	}

	delete m;
}

int Engine::Update()
{
	
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	if (this->windowHeight != height || this->windowWidth != width) {
		// window size changed:
		this->windowHeight = height;
		this->windowWidth = width;

		// realocate frame buffers:
		delete this->depthBuffer;
		this->depthBuffer = new float[(size_t)height * width];
	}

	// update proj and view matricies:
	View = camera.getViewMatrix();
	Projection = glm::perspective(1.5f, (float)width / (float)height, 2.5f, 200.0f);
	transform = Projection * View;

	// calculate time between two updated (frames)
	double currentTime = glfwGetTime();
	this->frameDelta = currentTime - this->lastFrameTime;
	this->lastFrameTime = currentTime;
	this->gui.guiUpdate(this->window);
	
	// update units positions:
	gameLogic.update(frameDelta);

	// draw user interface:
	userInterface.Draw(glm::mat4(1));

	// read depth buffer:
	glReadPixels(
		0, 0,						// starting corner
		windowWidth, windowHeight,  // rectangle size
		GL_DEPTH_COMPONENT,         // buffer type
		GL_FLOAT,                   // data type
		depthBuffer               // buffer address
	);


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

Engine::Engine() {
	this->transform = glm::mat4(1);
	this->windowWidth = 800;
	this->windowHeight = 600;
}
