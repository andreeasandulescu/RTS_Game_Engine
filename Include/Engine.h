#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Camera.h>
#include <thread>
#include <InputManager.h>
#include <MessageBus.h>
#include <GameMap.h>
#include <GUI.h>
#include <stb_image.h>
#include <water.h>
#include <LightSource.h>
#include <GameLogic.h>
#include <exception>

#ifndef _ENGINE_H_
#define _ENGINE_H_

class Engine : public MessageReceiver
{
	float lastFrameTime;
	float frameDelta;
public:
	Camera camera;
	GLFWwindow* window;
	MessageBus mainMessageBus;
	InputManager inputManager;
	Water water;
	GUI gui;
	Unit testUnit;
	GameLogic gameLogic;
	GameMap gameMap;

	std::vector<LightSource*> lightSources;
	

	std::thread mainBusThread;
	std::thread inputManagerThread;

	// contains the Projection * View
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 transform;

	// window width/height:
	int windowWidth;
	int windowHeight;

	// frame buffers data:
	float* depthBuffer;

	int Init();
	int Update();
	float getFrameDelta();

	// receive messages from the InputManager key and cursors:
	void receiveMessage(Message* m);
	
	int Stop();
	Engine();
};

#endif //_ENGINE_H_