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
					}
				}
				messageBus->addMessage(response);
			}
		}
	}

	// discard message after reading
	delete m;
}

void UserInterface::UpdateMesh() {
	if (currentScene)
		currentScene->UpdateMesh();
}

void UserInterface::Draw(const glm::mat4& transform) {
	// draw current scene
	if (currentScene)
		currentScene->Draw(transform);
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

	// create main menu:
	SceneUI* mainScene = new SceneUI(window, gui);
	mainScene->addButtonAlligned(ButtonAction::playAction, std::string("Play"), glm::vec3(153, 50, 204) / 300.0f, 1.0f);
	mainScene->addButtonAlligned(ButtonAction::unkownAction, std::string("Options"), glm::vec3(153, 50, 204) / 300.0f, 1.0f);
	mainScene->addButtonAlligned(ButtonAction::exitAction, std::string("Quit"), glm::vec3(153, 50, 204) / 300.0f, 1.0f);
	mainScene->UpdateMesh();
	
	// create obtions menu:
	SceneUI* optionsScene = new SceneUI(window, gui);
	optionsScene->addButtonAlligned(ButtonAction::loadMap1, std::string("Map 1"), glm::vec3(153, 50, 204) / 300.0f, 0.5f);
	optionsScene->addButtonAlligned(ButtonAction::loadMap2, std::string("Map 2"), glm::vec3(153, 50, 204) / 300.0f, 0.5f);
	optionsScene->addButtonAlligned(ButtonAction::loadMap3, std::string("Map 3"), glm::vec3(153, 50, 204) / 300.0f, 0.5f);
	optionsScene->addButtonAlligned(ButtonAction::unkownAction, std::string("Main Menu"), glm::vec3(153, 50, 204) / 300.0f, 0.5f);
	optionsScene->UpdateMesh();

	// add next scenes pointers:
	optionsScene->buttons[3]->m->nextScene = mainScene;
	mainScene->buttons[1]->m->nextScene = optionsScene;

	this->currentScene = mainScene;
}