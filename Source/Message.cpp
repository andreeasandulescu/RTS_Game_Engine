#include "Message.h"

std::atomic< int> Message::globalIds = 0;

Message::Message()
{
	this->id = Message::globalIds;
	Message::globalIds++;
	this->messageString = std::string("Unnamed message.");
}

Message::Message(std::string messageString)
{
	this->id = Message::globalIds;
	Message::globalIds++;
	this->messageString = messageString;
}

CursorMessage::CursorMessage(float xoffset, float yoffset, float lastX, float lastY) {
	this->xoffset = xoffset;
	this->yoffset = yoffset;
	this->lastX = lastX;
	this->lastY = lastY;

	char msgName[100];
	sprintf_s(msgName, "Mouse offset: [%f,%f] coords: [%f,%f].\n",
		this->xoffset,
		this->yoffset,
		this->lastX,
		this->lastY
	);

	this->messageString = std::string(msgName);
}
