#pragma once

#include <vector>
#include <list>
#include <unordered_set>

#include "Agent.h"
#include "Messages.h"
#include "Blackboard.h"

#include "LinkPathFinder.h"

namespace sf
{
	class RenderWindow;
}

class World;
class BaseObject;
class ReachabilityCluster;
class WindowObject;
class ReachabilityCluster;

// --------------------------------------------
// SMAHeat
// --------------------------------------------
class SMAHeat
{
	typedef std::vector<Agent*> AgentList;
	typedef std::vector<ReachabilityCluster*> ReachabilityClusters;
public:
	SMAHeat(void);
	~SMAHeat(void);

	void Init(World & parWorld);
	void Update(World & parWorld);
	void Draw(sf::RenderWindow& app) const;
	void BuildVisionCache(World const & parWorld);
	void BuildVisionCache(World const & parWorld, Agent* parAgent);

	// Creation des agents
	void CreateAgent(Probe & parProbe);
	void CreateAgent(HeatObject & parHeat);
	void CreateAgent(WindowObject & parHeat);
	void CreateAgent(BaseObject & parObject);

	int GetDistanceBetween(Agent const * agent1, Agent const * agent2);
private:
	void RegisterInVisionCache(Agent * parAgent, BaseObject const * parObject);
	AgentList       FDistributorAgent;
	AgentList       FEnvironmentalAgent;
	AgentList       FDecisionalAgent;
	Blackboard      FBlackboard;
	ReachabilityClusters  FReachabilityClusters;
	AgentList		FAgentToCacheInCluster;
	LinkPathFind    FLinkPathFinder;
	
};
