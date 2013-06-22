#pragma once

class Agent;

// --------------------------------------------
// Message
// --------------------------------------------
struct Message
{
	Agent * source;
	virtual void Decode(Agent * parAgent) = 0;
};

// --------------------------------------------
// TempDemand
// --------------------------------------------
struct TempDemand : public Message
{
	float InitialTemp;
	float WishTemp;
	virtual void Decode(Agent * parAgent);
};
