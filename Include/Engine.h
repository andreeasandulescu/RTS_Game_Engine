#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Engine
{
public:
	GLFWwindow* window;

	int Init();
	int Stop();
};

