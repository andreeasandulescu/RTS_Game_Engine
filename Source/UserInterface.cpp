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
	SceneUI* scene = new SceneUI(window, gui);
	scene->addButtonAlligned(ButtonAction::playAction, std::string("Play"), glm::vec3(153, 50, 204) / 300.0f, 1.0f);
	scene->addButtonAlligned(ButtonAction::unkownAction, std::string("Options"), glm::vec3(153, 50, 204) / 300.0f, 1.0f);
	scene->addButtonAlligned(ButtonAction::exitAction, std::string("Quit"), glm::vec3(153, 50, 204) / 300.0f, 1.0f);
	scene->UpdateMesh();
	
	this->currentScene = scene;
}