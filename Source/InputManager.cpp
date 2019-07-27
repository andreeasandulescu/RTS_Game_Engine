#include <InputManager.h>

void InputManager::listening() {
	// poll for mouse movement
	double xpos, ypos;
	while (1) {
		glfwGetCursorPos(window, &xpos, &ypos);

		xoffset = xpos - lastX;
		yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		// send message through the bus:
		CursorMessage *m = new CursorMessage(xoffset, yoffset, lastX, lastY);
		this->messageBus->addMessage(m);

		// change this to increase/decrease mouse sample rate:
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}

InputManager::InputManager() {

}

InputManager::InputManager(GLFWwindow* w, MessageBus* mb) {
	this->window = w;
	this->messageBus = mb;
}

InputManager::~InputManager() {

}