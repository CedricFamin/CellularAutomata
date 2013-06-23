#pragma once


// --------------------------------------------
// Forward
// --------------------------------------------
class Blackboard;
class HeatObject;
class Probe;
struct Message;
struct TempDemand;
class SMAHeat;

// --------------------------------------------
// Agent
// --------------------------------------------
class Agent
{
public:
	Agent(SMAHeat * parSMA, int parX, int parY) : FSMA(parSMA), x(parX), y(parY) { }
	virtual ~Agent() { }
	virtual void ReadAndWriteSomething(Blackboard & parBlackBoard) = 0;
	virtual void Read(Message* parMessage) {}
	virtual void Read(TempDemand* parMessage) {}

	int X() const { return x; }
	int Y() const { return y; }
protected:
	SMAHeat * FSMA;
	int x;
	int y;

};

// --------------------------------------------
// DistributorAgent
// --------------------------------------------
class DistributorAgent : public Agent
{
public:
	DistributorAgent(SMAHeat * parSMA, HeatObject & parHeat);
	virtual ~DistributorAgent();
	virtual void ReadAndWriteSomething(Blackboard & parBlackBoard);
	virtual void Read(TempDemand* parMessage);
private:
	HeatObject & FHeatObject;
};

// --------------------------------------------
// EnvironmentalAgent
// --------------------------------------------
class EnvironmentalAgent : public Agent
{
public:
	EnvironmentalAgent(SMAHeat * parSMA, Probe & parProbe);
	virtual ~EnvironmentalAgent();
	virtual void ReadAndWriteSomething(Blackboard & parBlackBoard);
private:
	Probe const & FProbe;
};


