#include "Message.h"

std::atomic< int> Message::globalIds = 0;

Message::Message()
{
	this->id = Message::globalIds;
	Message::globalIds++;
	this->messageString = std::string("Unnamed message.");
	this->messageType = MessageType::unkown;
}

Message::Message(const std::string& messageString)
{
	this->id = Message::globalIds;
	Message::globalIds++;
	this->messageString = messageString;
}

Message::Message(const Message& rhs)
{
	this->messageString = rhs.messageString;
	this->messageType = rhs.messageType;
	this->id = rhs.id;
}

Message* Message::clone()
{
	return new Message(*this);
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
	this->messageType = MessageType::cursor;
}

CursorMessage::CursorMessage(const CursorMessage& rhs)
	: Message(rhs)  // calls copy ctor of List class
{
	this->xoffset = rhs.xoffset;
	this->yoffset = rhs.yoffset;
	this->lastX = rhs.lastX;
	this->lastY = rhs.lastY;
	this->cursorState = rhs.cursorState;
}

CursorMessage* CursorMessage::clone()
{
	return new CursorMessage(*this);
}

KeysMessage::KeysMessage() : Message()
{
	this->pressedKeys = std::vector<unsigned int>();
	this->messageType = MessageType::keyspressed;
}

KeysMessage::KeysMessage(const KeysMessage& rhs)
	: Message(rhs)  // calls copy ctor of List class
{
	this->pressedKeys = rhs.pressedKeys;
	
}

KeysMessage* KeysMessage::clone()
{
	return new KeysMessage(*this);
}