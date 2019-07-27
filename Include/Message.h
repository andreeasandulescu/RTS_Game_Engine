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

#endif // !_MESSAGE_H_