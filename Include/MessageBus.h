#include <Message.h>
#include <queue>
#include <list>
#include <MessageReceiver.h>
#include <mutex>
#include <condition_variable> // std::condition_variable

#ifndef _MESSAGE_BUS_H_
#define _MESSAGE_BUS_H_

class MessageBus {
	// message FIFO
	std::queue<Message*, std::list<Message*>> messageQueue;

	// components subscribing to this message bus
	std::vector<MessageReceiver*> messageReceivers;

	// multiple producers (subscribers posting messages) - 1 consumer (message bus thread) 
	std::mutex messageQueueLock;
	std::mutex mtx;
	std::condition_variable cv;

public:
	// add a message to be broadcasted to receivers
	void addMessage(Message *m);

	// subscribe a receiveri to this message bus
	void addSubscriber(MessageReceiver* receiver);

	// message distributer loop
	// pops messages from the messages queue
	// sends them to the subscribers
	void sendMessages();

	bool hasMessages();

	MessageBus();
};

#endif // !_MESSAGE_BUS_H_