#include <iostream>
#include <string>
#include <atomic>
#include <vector>

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

enum MessageType { unkown, cursor, keyspressed };

class Message {
private:
	static std::atomic< int> globalIds;
public:
	unsigned int id;
	MessageType messageType;
	std::string messageString;

	Message();
	// copy constructor
	Message(const Message& rhs);

	virtual Message* clone();

	Message(std::string messageString);
};

class CursorMessage : public Message {
public:
	float xoffset;
	float yoffset;

	// last cursor positions
	float lastX;
	float lastY;

	virtual CursorMessage* clone();

	// copy constructor
	CursorMessage(const CursorMessage& rhs);
	CursorMessage(float xoffset, float yoffset, float lastX, float lastY);
};

class KeysMessage : public Message {
public:
	std::vector<unsigned int> pressedKeys;

	virtual KeysMessage* clone();

	// copy constructor
	KeysMessage();
	KeysMessage(const KeysMessage& rhs);
	
};

#endif // !_MESSAGE_H_