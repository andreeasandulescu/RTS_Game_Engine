#include <Message.h>

class MessageObserver {
	std::string observerName;

	virtual void notifyMe(Message m) = 0;
};