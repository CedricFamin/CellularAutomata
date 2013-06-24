#pragma once

#include <vector>
#include <unordered_set>

class Agent;
class Link;
namespace sf {
	class RenderWindow;
}

class ReachabilityCluster
{
public:
	typedef std::vector<Link*> Links;
	typedef std::unordered_set<Agent const*> AgentList;
	ReachabilityCluster(int parMinX, int parMaxX, int parMinY, int parMaxY);
	void Draw(sf::RenderWindow& app) const;

	int MinX() const { return FMinX; }
	int MaxX() const { return FMaxX; }
	int MinY() const { return FMinY; }
	int MaxY() const { return FMaxY; }
	void AddAgent(Agent const * parAgent) { FAgents.insert(parAgent); }
	bool Contains(Agent const * parAgent) const { return FAgents.find(parAgent) != FAgents.end(); }
	bool Contains(int parX, int parY) const { return FMinX <= parX && parX < FMaxX && FMinY <= parY && parY < FMaxY; }
	void AddLink(Link * parLink) { FLinks.push_back(parLink); }

	Links GetLinks() { return FLinks; }
	AgentList GetAgents() { return FAgents; }
private:
	int FMinX;
	int FMaxX;
	int FMinY;
	int FMaxY;
	AgentList FAgents;
	Links FLinks;
};