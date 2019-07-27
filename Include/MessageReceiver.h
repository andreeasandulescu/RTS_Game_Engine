#include <Message.h>

#ifndef _MESSAGE_RECEIVER_H_
#define _MESSAGE_RECEIVER_H_

class MessageReceiver {
public:
	virtual void receiveMessage(Message *m) = 0;
};

#endif