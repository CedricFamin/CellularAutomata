#include "SMAHeat.h"

#include "World.h"

// --------------------------------------------------------
// Agent
// --------------------------------------------------------
Agent::Agent()
	: FTemperature(0.0f)
{
}

Agent::~Agent()
{
}

Demand * Agent::MakeWish()
{
	FInternalDemand.Value = FTempToReach - FTemperature;
	return &FInternalDemand;
}

void Agent::ClearDemands()
{
	FExternalDemands.clear();
	FInternalDemand.Value = 0.0f;
	FInternalDemand.Destinataire = this;
}
// --------------------------------------------------------
// SMAHeat
// --------------------------------------------------------

SMAHeat::SMAHeat(void)
{
	FAgentGrid.resize(FCoordConverter.GetY());
	for (std::vector<Agent>& line : FAgentGrid)
	{
		FAgentGrid.resize(FCoordConverter.GetX());
	}
}


SMAHeat::~SMAHeat(void)
{
}

void SMAHeat::Update(World & parWorld)
{
	
}