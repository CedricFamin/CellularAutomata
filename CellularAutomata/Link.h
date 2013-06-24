#pragma once

#include <vector>
#include <unordered_map>

class ReachabilityCluster;
class Agent;

struct LinkWithDist
{ 
	LinkWithDist(Link * parLink, int parDist) : link(parLink), dist(parDist) {}
	Link * link; 
	unsigned int dist; 
};
struct AgentWithDist
{
	AgentWithDist(Agent const * parAgent, int parDist) : agent(parAgent), dist(parDist) {}
	Agent const * agent;
	unsigned int dist;
};

class Link 
{
public:
	Link(ReachabilityCluster * parCluster, int parX, int parY) : Cluster(parCluster), x(parX), y(parY) {}
	ReachabilityCluster * Cluster;
	int x;
	int y;
	std::vector<LinkWithDist> ReachableLinks;
	std::vector<AgentWithDist> ReachableAgents;
};
