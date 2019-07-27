#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Camera {
	float lastX;
	float lastY;
	float yaw;
	float pitch;

public:
	glm::vec3 cameraFront;
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;

	glm::mat4 getViewMatrix(glm::vec3 target);
	glm::mat4 getViewMatrix();
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void key_callback(GLFWwindow* window);

	Camera();
	~Camera();
};