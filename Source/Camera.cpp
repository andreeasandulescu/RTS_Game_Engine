#include "Camera.h"


glm::mat4 Camera::getViewMatrix(glm::vec3 target)
{
	return glm::lookAt(this->cameraPos, target, this->cameraUp);
}

glm::mat4 Camera::getViewMatrix()
{
	
	return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

void Camera::move_cursor(double xoffset, double yoffset)
{
	float sensitivity = 0.004f;
	float xos = sensitivity * (float)xoffset;
	float yos = sensitivity * (float)yoffset;

	cameraFront = cameraFront + cameraUp * yos;
	cameraFront = cameraFront + glm::normalize(glm::cross(cameraFront, cameraUp)) * xos;
	cameraFront = glm::normalize(cameraFront);
}


void Camera::receiveMessage(Message *m) {
	CursorMessage *cm;
	KeysMessage* km;

	if (m->messageType == MessageType::cursor) {
		cm = (CursorMessage*)m;
		move_cursor(cm->xoffset, cm->yoffset);
	}
	else if (m->messageType == MessageType::keyspressed) {
		km = (KeysMessage*)m;
		
		// iterate through pressed keys and change camera positions:
		for (std::vector<unsigned int>::iterator it = km->pressedKeys.begin(); it != km->pressedKeys.end(); ++it) {
			float cameraSpeed = 0.1f; // adjust accordingly

			switch (*it) {
				case GLFW_KEY_W:
					cameraPos += cameraSpeed * cameraFront;
					break;
				case GLFW_KEY_S:
					cameraPos -= cameraSpeed * cameraFront;
					break;
				case GLFW_KEY_A:
					cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
					break;
				case GLFW_KEY_D:
					cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
					break;
				default:
					break;
			}	
		}
	}
	delete m;
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

