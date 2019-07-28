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

		// poll for keys:
		KeysMessage* keysMessage = new KeysMessage();
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			keysMessage->pressedKeys.push_back(GLFW_KEY_W);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			keysMessage->pressedKeys.push_back(GLFW_KEY_S);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			keysMessage->pressedKeys.push_back(GLFW_KEY_A);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			keysMessage->pressedKeys.push_back(GLFW_KEY_D);

		this->messageBus->addMessage(keysMessage);

		// change this to increase/decrease mouse/keys sample rate:
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