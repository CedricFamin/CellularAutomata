#include "SMAHeat.h"

#include "World.h"

// --------------------------------------------------------
// SMAHeat
// --------------------------------------------------------

SMAHeat::SMAHeat(void)
{
}

// --------------------------------------------------------
// --------------------------------------------------------
SMAHeat::~SMAHeat(void)
{
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::Update(World & parWorld)
{
	FBlackboard.Clear();
	for (auto agent : FEnvironmentalAgent)
	{
		agent->ReadAndWriteSomething(FBlackboard);
	}

	for (auto agent : FDistributorAgent)
	{
		agent->ReadAndWriteSomething(FBlackboard);
	}
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::CreateAgent(Probe & parProbe)
{
	FEnvironmentalAgent.push_back(new EnvironmentalAgent(parProbe));
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::CreateAgent(HeatObject & parHeat)
{
	FDistributorAgent.push_back(new DistributorAgent(parHeat));
}


// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::CreateAgent(BaseObject & parObject)
{
	// Nothing
}