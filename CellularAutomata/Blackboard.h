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
	void Clear() { FMessages.clear(); }
private:
	MessageList FMessages;
};