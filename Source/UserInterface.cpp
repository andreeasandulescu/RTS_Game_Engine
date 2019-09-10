#include <UserInterface.h>

void UserInterface::receiveMessage(Message* m) {

	Message* response = NULL;
	Cursor c;


	if (currentScene != NULL) {
		if (m->messageType == MessageType::cursor) {
			CursorMessage* c = (CursorMessage*)m;
			if (c->cursorState.left == ButtonStatus::PRESSED) {
				response = currentScene->handleClickEvent(c->cursorState);
				
			}

			if (response == NULL) {
				// no button was pressed in the current scene:
			}
			else {
				if (response->messageType == MessageType::buttonpress) {
					// check if button had a next scene attached:
					ButtonPressed* btnMsg = (ButtonPressed*)response;
					if (btnMsg->nextScene != NULL) {
						currentScene = (SceneUI*)btnMsg->nextScene;
						currentScene->resourceLoader = resourceLoader;
					}
				}
				messageBus->addMessage(response);
			}
		}
	}

	// discard message after reading
	delete m;
}

void UserInterface::UpdateMesh(ResourceLoader* resourceLoader) {
	this->resourceLoader = resourceLoader;
	if (currentScene)
		currentScene->UpdateMesh(resourceLoader);
}

void UserInterface::Draw(const glm::mat4& transform) {
	// draw current scene
	if (currentScene) {
		currentScene->resourceLoader = resourceLoader;
		currentScene->Draw(transform);
	}
}

void UserInterface::Draw(const glm::mat4& transform, const std::vector<LightSource*>& lightSources, glm::vec3 cameraPos) {

}
UserInterface::UserInterface() {
	currentScene = NULL;
	window = NULL;
	messageBus = NULL;
}

void UserInterface::initUserInterface(GLFWwindow* window, GUI* gui, MessageBus* messageBus) {

	this->window = window;
	this->messageBus = messageBus;
	this->gui = gui;

	glm::vec3 textColor = glm::vec3(153, 50, 204) / 300.0f;

	// create main menu:
	SceneUI* mainScene = new SceneUI(window, gui);
	mainScene->addButtonAlligned(ButtonAction::playAction, std::string("Play"), textColor, 1.0f);
	mainScene->addButtonAlligned(ButtonAction::unkownAction, std::string("Options"), textColor, 1.0f);
	mainScene->addButtonAlligned(ButtonAction::exitAction, std::string("Quit"), textColor, 1.0f);
	mainScene->UpdateMesh(resourceLoader);
	
	// create obtions menu:
	SceneUI* optionsScene = new SceneUI(window, gui);
	optionsScene->addButtonAlligned(ButtonAction::loadMap1, std::string("Map 1"), textColor, 0.5f);
	optionsScene->addButtonAlligned(ButtonAction::loadMap2, std::string("Map 2"), textColor, 0.5f);
	optionsScene->addButtonAlligned(ButtonAction::loadMap3, std::string("Map 3"), textColor, 0.5f);
	optionsScene->addButtonAlligned(ButtonAction::unkownAction, std::string("Main Menu"), textColor, 0.5f);
	optionsScene->UpdateMesh(resourceLoader);

	// create in-game user interface:
	SceneUI* playScene = new SceneUI(window, gui);
	playScene->addButtonBorderAlligned(ButtonAction::unkownAction, std::string("Builder"), textColor, 0.5f);
	playScene->addButtonBorderAlligned(ButtonAction::house1, std::string("House"), textColor, 0.5f);
	playScene->addButtonBorderAlligned(ButtonAction::woodWall, std::string("Wood Wall"), textColor, 0.5f);
	playScene->UpdateMesh(resourceLoader);

	// add next scenes pointers:
	optionsScene->buttons[3]->m->nextScene = mainScene;
	mainScene->buttons[1]->m->nextScene = optionsScene;
	mainScene->buttons[0]->m->nextScene = playScene;

	this->currentScene = mainScene;
}