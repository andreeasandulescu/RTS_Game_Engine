#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <MessageReceiver.h>



class Camera : public MessageReceiver {
	float lastX;
	float lastY;
	float yaw;
	float pitch;
	
public:
	glm::vec3 cameraFront;
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;

	bool strategic;

	glm::mat4 getViewMatrix(glm::vec3 target);
	glm::mat4 getViewMatrix();
	void move_cursor(double xoffset, double yoffset);
	void receiveMessage(Message *m);

	Camera();
	~Camera();
};

#endif // _CAMERA_H_