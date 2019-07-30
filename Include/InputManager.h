
#include <GLFW/glfw3.h>
#include <Message.h>
#include <MessageBus.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::milliseconds

#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

class InputManager {
	GLFWwindow *window;
	MessageBus *messageBus;

	// cursor offsets since last update:
	float xoffset;
	float yoffset;

	// last cursor positions
	float lastX;
	float lastY;

public:

	/*
		Poll for window events.
		Put messages for the camera in the queue.
	*/
	void listening();
	InputManager();
	InputManager(GLFWwindow* w, MessageBus* mb);
	~InputManager();
};

#endif //_INPUT_MANAGER_H_