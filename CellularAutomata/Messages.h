#pragma once

class Agent;
class Blackboard;

// --------------------------------------------
// Message
// --------------------------------------------
struct Message
{
    virtual ~Message() {}
	Agent * source;
	virtual void Decode(Blackboard & parBlackboard, Agent * parAgent) = 0;
};

// --------------------------------------------
// TempDemand
// --------------------------------------------
struct TempDemand : public Message
{
	float InitialTemp;
	float WishTemp;
	virtual void Decode(Blackboard & parBlackboard, Agent * parAgent);
};

// --------------------------------------------
// HeatProposition
// --------------------------------------------
struct HeatProposition : public Message
{
	unsigned int DistWithDestination;
	TempDemand const * Demand;
	virtual void Decode(Blackboard & parBlackboard, Agent * parAgent);
	void Execute();
};

// --------------------------------------------
// ColdProposition
// --------------------------------------------
struct ColdProposition : public Message
{
	unsigned int DistWithDestination;
	TempDemand const * Demand;
	virtual void Decode(Blackboard & parBlackboard, Agent * parAgent);
	void Execute();
};

