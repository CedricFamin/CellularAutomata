#pragma once

#include <vector>
#include <list>

#include "CoordConverter.h"

class Agent;
class World;

struct Demand
{
	float Value;
	Agent const * Destinataire;
};

class Agent
{
public:
	typedef std::vector<Demand> Demands;
	Agent();
	~Agent();
	Demand * MakeWish();
	void ClearDemands();
	void SetTemperature(float parTemp) { FTemperature = parTemp; }
protected:
private:
	Demands FExternalDemands;
	Demand  FInternalDemand;
	float FTempToReach;
	float FTemperature;
};

class SMAHeat
{
	typedef std::vector<std::vector<Agent>> AgentGrid;
public:
	SMAHeat(void);
	~SMAHeat(void);

	CoordConverter const & GetCoordConverter() const { return FCoordConverter; }
	void Update(World & parWorld);
private:
	CoordConverter	FCoordConverter;
	AgentGrid		FAgentGrid;
};

