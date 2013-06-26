#include "Messages.h"

#include "Agent.h"
#include "Blackboard.h"

void TempDemand::Decode(Blackboard & parBlackboard, Agent * parAgent)
{
	parAgent->Read(parBlackboard, this);
}

void HeatProposition::Decode(Blackboard & parBlackboard, Agent * parAgent)
{
	parAgent->Read(parBlackboard, this);
}

void  HeatProposition::Execute()
{
	source->Execute();
}

void ColdProposition::Decode(Blackboard & parBlackboard, Agent * parAgent)
{
	parAgent->Read(parBlackboard, this);
}

void  ColdProposition::Execute()
{
	source->Execute();
}
