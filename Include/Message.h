#include <iostream>
#include <string>
#include <atomic>
#include <vector>
#include <Cursor.h>
#include <glm/gtc/type_ptr.hpp>

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

enum MessageType { unkown, cursor, keyspressed, worldclick, buttonpress };
enum ButtonAction { unkownAction, playAction, exitAction, loadMap1, loadMap2, loadMap3, house1, woodWall, strategicCamera};

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

	Message(const std::string& message);
};

class CursorMessage : public Message {
public:
	Cursor cursorState;

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

class WorldClick : public Message {
public:
	virtual WorldClick* clone();

	Cursor cursorState;
	glm::vec3 worldPosition;

	// copy constructor
	WorldClick();
	WorldClick(const WorldClick& rhs);
};

class ButtonPressed : public Message {
public:
	virtual ButtonPressed* clone();

	void* nextScene;
	ButtonAction action;

	// copy constructor
	ButtonPressed();
	ButtonPressed(const ButtonPressed& rhs);
};

#endif // !_MESSAGE_H_