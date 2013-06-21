#pragma once

class Agent;

// --------------------------------------------
// Message
// --------------------------------------------
struct Message
{
	Agent * source;
};

// --------------------------------------------
// TempDemand
// --------------------------------------------
struct TempDemand : public Message
{
	float InitialTemp;
	float WishTemp;
};
