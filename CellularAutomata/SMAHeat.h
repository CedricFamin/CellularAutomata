#pragma once

#include <vector>
#include <list>
#include <unordered_set>

#include "CoordConverter.h"
#include "Agent.h"
#include "Messages.h"
#include "Blackboard.h"

namespace sf
{
	class RenderWindow;
}

class World;
class BaseObject;
class VisionCluster;

struct Link {
	Link(VisionCluster * parCluster, int parX, int parY) : Cluster(parCluster), x(parX), y(parY) {}
	VisionCluster * Cluster;
	int x;
	int y;
};

class VisionCluster
{
public:
	typedef std::vector<Link> Links;
	typedef std::unordered_set<Agent const*> AgentList;
	VisionCluster(int parMinX, int parMaxX, int parMinY, int parMaxY);
	void Draw(sf::RenderWindow& app) const;

	int MinX() const { return FMinX; }
	int MaxX() const { return FMaxX; }
	int MinY() const { return FMinY; }
	int MaxY() const { return FMaxY; }
	void AddAgent(Agent const * parAgent) { FAgents.insert(parAgent); }
	bool Contains(Agent const * parAgent) const { FAgents.find(parAgent) != FAgents.end(); }
	bool Contains(int parX, int parY) const { return FMinX <= parX && parX < FMaxX && FMinY <= parY && parY < FMaxY; }
	void AddLink(Link const & parLink) { FLinks.push_back(parLink); }
private:
	int FMinX;
	int FMaxX;
	int FMinY;
	int FMaxY;
	AgentList FAgents;
	Links FLinks;
};
// --------------------------------------------
// SMAHeat
// --------------------------------------------
class SMAHeat
{
	typedef std::vector<Agent*> AgentList;
	typedef std::vector<VisionCluster*> VisionClusters;
public:
	SMAHeat(void);
	~SMAHeat(void);

	void Init(World & parWorld);
	void Update(World & parWorld);
	void Draw(sf::RenderWindow& app) const;
	void BuildVisionCache(World const & parWorld);

	// Creation des agents
	void CreateAgent(Probe & parProbe);
	void CreateAgent(HeatObject & parHeat);
	void CreateAgent(BaseObject & parObject);

	int GetDistanceBetween(Agent const * agent1, Agent const * agent2);
private:
	void RegisterInVisionCache(Agent * parAgent, BaseObject const * parObject);
	CoordConverter	FCoordConverter;
	AgentList       FDistributorAgent;
	AgentList       FEnvironmentalAgent;
	Blackboard      FBlackboard;
	VisionClusters  FVisionClusters;
	
};
