#include "Blackboard.h"

// --------------------------------------------
// BlackBoard
// --------------------------------------------
void Blackboard::Clear()
{
	for (Message* msg : FMessages)
	{
		delete msg;
	}
	FMessages.clear();
}

// --------------------------------------------
// --------------------------------------------
void Blackboard::Write(Message * parMessage)
{
	FMessages.push_back(parMessage);
}
