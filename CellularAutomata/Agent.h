#pragma once

#include <map>

// --------------------------------------------
// Forward
// --------------------------------------------
class Blackboard;
class HeatObject;
class Probe;
struct Message;
struct TempDemand;
struct HeatProposition;
struct ColdProposition;
class SMAHeat;
class WindowObject;

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
	virtual void Read(Blackboard & parBlackBoard, TempDemand* parMessage) {}
	virtual void Read(Blackboard & parBlackBoard, HeatProposition* parMessage) {}
	virtual void Read(Blackboard & parBlackBoard, ColdProposition* parMessage) {}
	virtual void Execute() {}

	int X() const { return x; }
	int Y() const { return y; }
protected:
	SMAHeat * FSMA;
	int x;
	int y;

};

// --------------------------------------------
// DistributorHeatAgent
// --------------------------------------------
class DistributorHeatAgent : public Agent
{
public:
	DistributorHeatAgent(SMAHeat * parSMA, HeatObject & parHeat);
	virtual ~DistributorHeatAgent();
	virtual void ReadAndWriteSomething(Blackboard & parBlackBoard);
	virtual void Read(Blackboard & parBlackBoard, TempDemand* parMessage);
	virtual void Execute();
private:
	HeatObject & FHeatObject;
};

// --------------------------------------------
// DistributorColdAgent
// --------------------------------------------
class DistributorColdAgent : public Agent
{
public:
	DistributorColdAgent(SMAHeat * parSMA, WindowObject & parObject);
	virtual ~DistributorColdAgent();
	virtual void ReadAndWriteSomething(Blackboard & parBlackBoard);
	virtual void Read(Blackboard & parBlackBoard, TempDemand* parMessage);
	virtual void Execute();
private:
	WindowObject & FWindowObject;
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

// --------------------------------------------
// DecisionalAgent
// --------------------------------------------
class DecisionalAgent : public Agent
{
public:
	DecisionalAgent(SMAHeat * parSMA);
	virtual ~DecisionalAgent();
	virtual void ReadAndWriteSomething(Blackboard & parBlackBoard);
	virtual void Read(Blackboard & parBlackBoard, HeatProposition* parMessage);
	virtual void Read(Blackboard & parBlackBoard, ColdProposition* parMessage);
private:
	std::map<TempDemand const * , HeatProposition*> FDemandWithBestProposition;
	std::map<TempDemand const * , ColdProposition*> FColdDemandWithBestProposition;
};

