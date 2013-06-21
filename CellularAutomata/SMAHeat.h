#pragma once

#include <vector>
#include <list>

#include "CoordConverter.h"
#include "Agent.h"
#include "Messages.h"
#include "Blackboard.h"

class World;
class BaseObject;

// --------------------------------------------
// SMAHeat
// --------------------------------------------
class SMAHeat
{
	typedef std::vector<Agent*> AgentList;
public:
	SMAHeat(void);
	~SMAHeat(void);

	void Update(World & parWorld);

	// Creation des agents
	void CreateAgent(Probe & parProbe);
	void CreateAgent(HeatObject & parHeat);
	void CreateAgent(BaseObject & parObject);
private:
	CoordConverter	FCoordConverter;
	AgentList FDistributorAgent;
	AgentList FEnvironmentalAgent;
	Blackboard FBlackboard;
	
};
