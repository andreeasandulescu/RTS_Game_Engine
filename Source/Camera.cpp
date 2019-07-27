#include "Camera.h"


glm::mat4 Camera::getViewMatrix(glm::vec3 target)
{
	return glm::lookAt(this->cameraPos, target, this->cameraUp);
}

glm::mat4 Camera::getViewMatrix()
{
	
	return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.004f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cameraFront = cameraFront + cameraUp * yoffset;
	cameraFront = cameraFront + glm::normalize(glm::cross(cameraFront, cameraUp)) * xoffset;
	cameraFront = glm::normalize(cameraFront);

	
}

void Camera::key_callback(GLFWwindow* window)
{
	float cameraSpeed = 0.1f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

Camera::Camera() {
	// default position
	this->cameraPos = glm::vec3(0.5f, 0.5f, 0.5f);
	
	// default direction is pointing toward the center of the scene
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	this->cameraFront = glm::normalize(cameraPos - cameraTarget);

	// cross product between up and direction will yeld right vector
	this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	lastX = 0;
	lastY = 0;
	yaw = 0;
	pitch = 0;
}

Camera::~Camera()
{
}

