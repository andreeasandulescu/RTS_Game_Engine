#include <InputManager.h>

void InputManager::listening() {
	// poll for mouse movement
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
}

InputManager::InputManager(GLFWwindow* w, MessageBus* mb) {
	this->window = w;
	this->messageBus = mb;
}

InputManager::~InputManager() {

}