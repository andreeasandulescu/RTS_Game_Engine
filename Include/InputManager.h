
#include <GLFW/glfw3.h>
#include <Message.h>
#include <MessageBus.h>

#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

class InputManager {
	GLFWwindow *window;
	MessageBus *messageBus;

public:

	/*
		Poll for window events.
		TODO: sync with framerate
		Put messages for the camera in the queue.
	*/
	void listening();
	InputManager(GLFWwindow* window, MessageBus* messageBus);
	~InputManager();
};

#endif //_INPUT_MANAGER_H_