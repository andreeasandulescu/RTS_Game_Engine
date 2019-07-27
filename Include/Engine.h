#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Camera.h>
#include <thread>
#include <InputManager.h>
#include <MessageBus.h>

#ifndef _ENGINE_H_
#define _ENGINE_H_

class Engine
{
	float lastFrameTime;
	float frameDelta;
public:
	Camera camera;
	GLFWwindow* window;
	MessageBus mainMessageBus;
	InputManager inputManager;
	
	std::thread mainBusThread;
	std::thread inputManagerThread;

	int Init();
	int Update();
	float getFrameDelta();
	int Stop();
	Engine();
};

#endif //_ENGINE_H_