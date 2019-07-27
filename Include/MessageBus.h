#include <Message.h>
#include <queue>

#ifndef _MESSAGE_BUS_H_

class MessageBus {

	std::queue<Message> messageQueue;
	

	MessageBus();
};

#endif // !_MESSAGE_BUS_H_