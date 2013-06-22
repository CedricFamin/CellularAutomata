#include "Messages.h"

#include "Agent.h"

void TempDemand::Decode(Agent * parAgent)
{
	parAgent->Read(this);
}