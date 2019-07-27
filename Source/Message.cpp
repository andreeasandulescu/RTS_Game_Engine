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
