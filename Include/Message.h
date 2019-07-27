#include <iostream>
#include <string>
#include <atomic>

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

class Message {
private:
	static std::atomic< int> globalIds;
public:
	unsigned int id;
	std::string messageString;

	Message();

	Message(std::string messageString);
};

class CursorMessage : public Message {
public:
	float xoffset;
	float yoffset;

	// last cursor positions
	float lastX;
	float lastY;
	CursorMessage(float xoffset, float yoffset, float lastX, float lastY);
};

#endif // !_MESSAGE_H_