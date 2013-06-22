#pragma once

#include <vector>
#include "Messages.h"

// --------------------------------------------
// BlackBoard
// --------------------------------------------
class Blackboard
{
public:
	typedef std::vector<Message*> MessageList;
	void Clear();
	void Write(Message * parMessage);
	// copie car les client peuvent inserer des messages durant le parcours
	MessageList GetMessages() { return FMessages; }
private:
	MessageList FMessages;
};