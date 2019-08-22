#include <InputManager.h>

ButtonStatus getCurrentButtonStatus(int lastState, int newState) {
	ButtonStatus status;
	// submit change to mouse:
	if (newState == lastState) {
		if (newState == GLFW_PRESS) {
			status = ButtonStatus::UNCHANGED_PRESSED;
		}
		else {
			status = ButtonStatus::UNCHANGED_RELEASED;
		}

	}
	else {
		if (newState == GLFW_PRESS) {
			status = ButtonStatus::PRESSED;
		}
		else {
			status = ButtonStatus::RELEASED;
		}
	}
	return status;
}

void InputManager::listening() {
	// poll for mouse movement
	double xpos, ypos;
	
	Cursor mouseState;

	int rightMouseButtonState = GLFW_RELEASE;
	int leftMouseButtonState = GLFW_RELEASE;

	int rightMouseButtonLastState = GLFW_RELEASE;
	int leftMouseButtonLastState = GLFW_RELEASE;

	while (1) {
		// poll for mouse position:
		glfwGetCursorPos(window, &xpos, &ypos);

		xoffset = xpos - lastX;
		yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		// poll for left click:
		leftMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		mouseState.left = getCurrentButtonStatus(leftMouseButtonLastState, leftMouseButtonState);
		leftMouseButtonLastState = leftMouseButtonState;

		// poll for right click:
		rightMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		mouseState.right = getCurrentButtonStatus(rightMouseButtonLastState, rightMouseButtonState);
		rightMouseButtonLastState = rightMouseButtonState;


		mouseState.xPos = xpos;
		mouseState.yPos = ypos;

		// send message through the bus:
		CursorMessage *m = new CursorMessage(xoffset, yoffset, lastX, lastY);
		m->cursorState = mouseState;
		this->messageBus->addMessage(m);

		// poll for keys:
		KeysMessage *keysMessage = new KeysMessage();
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