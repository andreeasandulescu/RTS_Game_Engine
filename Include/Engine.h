#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include <iostream>

class Engine
{
	float lastFrameTime;
	float frameDelta;
public:
	Camera camera;
	GLFWwindow* window;
	
	int Init();
	int Update();
	float getFrameDelta();
	int Stop();
};