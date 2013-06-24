#pragma once

#include <vector>

class ReachabilityCluster;
class Agent;

class Link 
{
public:
	Link(ReachabilityCluster * parCluster, int parX, int parY) : Cluster(parCluster), x(parX), y(parY) {}
	ReachabilityCluster * Cluster;
	int x;
	int y;
	std::vector<Link*> ReachableLinks;
	std::vector<Agent const*> ReachableAgents;
};
