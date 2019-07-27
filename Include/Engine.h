#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Camera.h>

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

