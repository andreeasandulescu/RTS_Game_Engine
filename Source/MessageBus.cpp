#include <MessageBus.h>

int messageQueueSize = 0;
bool messageQueueHasElements() {
	return messageQueueSize > 0;
}

void MessageBus::addMessage(Message *m) {
	messageQueueLock.lock();
	this->messageQueue.push(m);
	messageQueueSize = this->messageQueue.size();
	messageQueueLock.unlock();

	// notify messageBus thread a message has been pushed
	//std::unique_lock<std::mutex> lck(mtx);
	cv.notify_one();
}

void MessageBus::addSubscriber(MessageReceiver* receiver) {
	this->messageReceivers.push_back(receiver);
}

void MessageBus::sendMessages() {
	// pop messages from message bus and notify receivers
	while (1) {
		
		// wait for the message bus to receive items
		std::unique_lock<std::mutex> lck(mtx);
		cv.wait(lck, messageQueueHasElements);

		messageQueueLock.lock();
		Message *m = this->messageQueue.front();
		messageQueueSize = this->messageQueue.size();
		if (messageQueue.size() > 10) {
			printf("Queue flooding: %d", messageQueue.size());
		}
		messageQueueLock.unlock();

		// send message to all subscribers:
		std::vector<MessageReceiver*>::iterator it;
		for (it = this->messageReceivers.begin(); it != this->messageReceivers.end(); ++it) {
			MessageReceiver *receiver = *it;
			receiver->receiveMessage(m->clone());
		}

		// remove last message
		messageQueueLock.lock();
		this->messageQueue.pop();
		delete m;
		messageQueueSize--;
		messageQueueLock.unlock();
	}
}

bool MessageBus::hasMessages() {
	bool hasMsg = false;
	messageQueueLock.lock();
	if (messageQueue.size() > 0) {
		hasMsg = true;
	}
	messageQueueLock.unlock();
	return hasMsg;
}

MessageBus::MessageBus() {
	this->messageQueue = std::queue<Message*, std::list<Message*>>();
	this->messageReceivers = std::vector<MessageReceiver*>();
}

