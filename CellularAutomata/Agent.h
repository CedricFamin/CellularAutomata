#pragma once


// --------------------------------------------
// Forward
// --------------------------------------------
class Blackboard;
class HeatObject;
class Probe;

// --------------------------------------------
// Agent
// --------------------------------------------
class Agent
{
public:
	Agent() { }
	virtual ~Agent() { }
	virtual void ReadAndWriteSomething(Blackboard & parBlackBoard) = 0;

};

// --------------------------------------------
// DistributorAgent
// --------------------------------------------
class DistributorAgent : public Agent
{
public:
	DistributorAgent(HeatObject & parHeat) {}
	virtual ~DistributorAgent() {}
	virtual void ReadAndWriteSomething(Blackboard & parBlackBoard) {}
};

// --------------------------------------------
// EnvironmentalAgent
// --------------------------------------------
class EnvironmentalAgent : public Agent
{
public:
	EnvironmentalAgent(Probe & parProbe) {}
	virtual ~EnvironmentalAgent() {}
	virtual void ReadAndWriteSomething(Blackboard & parBlackBoard) {}
};


