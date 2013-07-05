#include <list>
#include <queue>

#include "SMAHeat.h"

#include "World.h"
#include "ReachablePathFind.h"
#include "ReachabilityCluster.h"
#include "Link.h"
#include "Utils.h"

namespace {
	static const int CLUSTER_SIZE = 10;
	static const float FCLUSTER_SIZE = static_cast<float>(CLUSTER_SIZE);
}


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
void SMAHeat::Init(World & parWorld)
{
	CellularAutomata const & celluls = parWorld.GetCelluls();
	PointI const & mapSize = celluls.GetSize();

	int nbCluster = mapSize.x * mapSize.y / 100;

	for (int i = 0; i < nbCluster; ++i)
	{
		int minPosX = (i % (mapSize.x / 10)) * 10;
		int minPosY = (i / (mapSize.x  / 10)) * 10;
		ReachabilityCluster * cluster = new ReachabilityCluster(minPosX, minPosX + 10, minPosY, minPosY + 10);
		FReachabilityClusters.push_back(cluster);
	}
	FDecisionalAgent.push_back(new DecisionalAgent(this));
}

// --------------------------------------------------------
// --------------------------------------------------------
bool CreateLinkRight(CellularAutomata const & celluls, ReachabilityCluster * c1, ReachabilityCluster * c2, int left, int right, int x)
{
	
	int begin = -1;
	while (left < right)
	{
		if (!celluls[left][x].IsWall && !celluls[left][x - 1].IsWall)
		{
			if (begin == -1)
				begin = left;
		}
		else if (begin >= 0)
		{
			int middle = (left - begin) / 2;
			Link * cLink1 = new Link(c2, x - 1, begin + middle);
			Link * cLink2 = new Link(c1, x, begin + middle);
			cLink1->ReachableLinks.push_back(LinkWithDist(cLink2, 1));
			cLink2->ReachableLinks.push_back(LinkWithDist(cLink1, 1));
			c1->AddLink(cLink1);
			c2->AddLink(cLink2);
			begin = -1;
		}
		++left;
	}
	if (begin >= 0)
	{
		int middle = (left - begin) / 2;
		Link * cLink1 = new Link(c2, x - 1, begin + middle);
		Link * cLink2 = new Link(c1, x, begin + middle);
		cLink1->ReachableLinks.push_back(LinkWithDist(cLink2, 1));
		cLink2->ReachableLinks.push_back(LinkWithDist(cLink1, 1));
		c1->AddLink(cLink1);
		c2->AddLink(cLink2);
		begin = -1;
	}
	return true;
}

// --------------------------------------------------------
// --------------------------------------------------------
void CreateLinkBot(CellularAutomata const & celluls, ReachabilityCluster * c1, ReachabilityCluster * c2, int left, int right, int y)
{
	
	int begin = -1;
	while (left < right)
	{
		if (!celluls[y][left].IsWall && !celluls[y - 1][left].IsWall)
		{
			if (begin == -1)
				begin = left;
		}
		else if (begin >= 0)
		{
			int middle = (left - begin) / 2;
			Link * cLink1 = new Link(c2, begin + middle, y - 1);
			Link * cLink2 = new Link(c1, begin + middle, y);
			cLink1->ReachableLinks.push_back(LinkWithDist(cLink2, 1));
			cLink2->ReachableLinks.push_back(LinkWithDist(cLink1, 1));
			c1->AddLink(cLink1);
			c2->AddLink(cLink2);
			begin = -1;
		}
		++left;
	}
	if (begin >= 0)
	{
		int middle = (left - begin) / 2;
		Link * cLink1 = new Link(c2, begin + middle, y - 1);
		Link * cLink2 = new Link(c1, begin + middle, y);
		cLink1->ReachableLinks.push_back(LinkWithDist(cLink2, 1));
		cLink2->ReachableLinks.push_back(LinkWithDist(cLink1, 1));
		c1->AddLink(cLink1);
		c2->AddLink(cLink2);
		begin = -1;
	}
}
	
// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::BuildVisionCache(World const & parWorld)
{
	std::cout << "Vuild Vision cache" << std::endl;
	CellularAutomata const & celluls = parWorld.GetCelluls();
	PointI const & mapSize = celluls.GetSize();

	std::cout << "build Cluster external link" << std::endl;
	for (unsigned int i = 0; i < FReachabilityClusters.size(); ++i)
	{
		ReachabilityCluster * currentCluster = FReachabilityClusters[i];
		// bot
		{
			unsigned int otherClusterIndex = i + 0 + 1 * mapSize.x / CLUSTER_SIZE;
			if (otherClusterIndex >= 0 && otherClusterIndex < FReachabilityClusters.size())
			{
				ReachabilityCluster * oCluster = FReachabilityClusters[otherClusterIndex];
				if (currentCluster->MaxX() >= mapSize.x) continue;
				CreateLinkBot(celluls, currentCluster, oCluster, currentCluster->MinX(), 
					currentCluster->MinX() + CLUSTER_SIZE, currentCluster->MaxY());
			}
		}

		// right
		{
			unsigned int otherClusterIndex = i + 1 + 0 * mapSize.x / CLUSTER_SIZE;
			if (otherClusterIndex >= 0 && otherClusterIndex < FReachabilityClusters.size())
			{
				ReachabilityCluster * oCluster = FReachabilityClusters[otherClusterIndex];
				if (currentCluster->MaxX() >= mapSize.x) continue;
				CreateLinkRight(celluls, currentCluster, oCluster, currentCluster->MinY(), 
					currentCluster->MinY() + CLUSTER_SIZE, currentCluster->MaxX());
			}
		}
	}

	std::cout << "build Cluster internal link <=> link" << std::endl;

	for (ReachabilityCluster * cluster : FReachabilityClusters)
	{
		ReachablePathFind pathFind(&celluls, cluster);
		// Reachabilite entre les different lien vers l'exterieur
		for (auto& link : cluster->GetLinks())
		{
			for (auto& link : cluster->GetLinks())
			{
				for (auto const & oLink : cluster->GetLinks())
				{
					if (link == oLink)
						continue;
					Position p1 = { link->x, link->y };
					Position p2 = { oLink->x, oLink->y };
					pathFind.Init(p1, p2);
					unsigned int dist = pathFind.ComputePath();
					if (dist)
					{
						link->ReachableLinks.push_back(LinkWithDist(oLink, dist));
					}
				}
			}
		}
	}
	std::cout << "build Cluster: End" << std::endl;
	std::vector<Link const *> links;
	for (ReachabilityCluster * cluster : FReachabilityClusters)
		for (auto& link : cluster->GetLinks())
			links.push_back(link);
	FLinkPathFinder.Init(links, mapSize.x, mapSize.y);
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::BuildVisionCache(World const & parWorld, Agent* parAgent)
{
	std::cout << "build Cluster internal link <=> Agent" << std::endl;
	ReachabilityCluster * cluster = NULL;
	for (auto const & currentCluster : FReachabilityClusters)
	{
		if (currentCluster->Contains(parAgent))
			cluster = currentCluster;
	}

	ReachablePathFind pathFind(&parWorld.GetCelluls(), cluster);
	// Reachabilite entre les different agent et les lien interieur
	for (auto& link : cluster->GetLinks())
	{
		for (auto& link : cluster->GetLinks())
		{
			Position p1 = { link->x, link->y };
			Position p2 = { parAgent->X(), parAgent->Y() };
			pathFind.Init(p1, p2);
			unsigned int dist = pathFind.ComputePath();
			if (dist)
			{
				link->ReachableAgents.push_back(AgentWithDist(parAgent, dist)); 
			}
		}
	}
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::Update(World & parWorld)
{
	for (Agent * agent : FAgentToCacheInCluster)
	{
		BuildVisionCache(parWorld, agent);
	}
	FAgentToCacheInCluster.clear();

	FBlackboard.Clear();
	for (auto agent : FEnvironmentalAgent)
	{
		agent->ReadAndWriteSomething(FBlackboard);
	}

	for (auto agent : FDistributorAgent)
	{
		agent->ReadAndWriteSomething(FBlackboard);
	}

	for (auto agent : FDecisionalAgent)
	{
		agent->ReadAndWriteSomething(FBlackboard);
	}
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::Draw(sf::RenderWindow& app) const
{
	sf::Vector2f clusterSize(100.0f, 100.0f);

	for (auto & cluster : FReachabilityClusters)
	{
		sf::RectangleShape rect(clusterSize);
		rect.setPosition(sf::Vector2f(cluster->MinX() * 10.0f, cluster->MinY() * 10.0f));
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::Red);
		rect.setOutlineThickness(-1.0f);
		app.draw(rect);
		cluster->Draw(app);
	}
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::CreateAgent(Probe & parProbe)
{
	FEnvironmentalAgent.push_back(new EnvironmentalAgent(this, parProbe));
	RegisterInVisionCache(FEnvironmentalAgent.back(), &parProbe);
	FAgentToCacheInCluster.push_back(FEnvironmentalAgent.back());
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::CreateAgent(HeatObject & parHeat)
{
	FDistributorAgent.push_back(new DistributorHeatAgent(this, parHeat));
	RegisterInVisionCache(FDistributorAgent.back(), &parHeat);
	FAgentToCacheInCluster.push_back(FDistributorAgent.back());
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::CreateAgent(WindowObject & parObject)
{
	FDistributorAgent.push_back(new DistributorColdAgent(this, parObject));
	RegisterInVisionCache(FDistributorAgent.back(), &parObject);
	FAgentToCacheInCluster.push_back(FDistributorAgent.back());
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::CreateAgent(BaseObject & parObject)
{
	// Nothing
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::RegisterInVisionCache(Agent * parAgent, BaseObject const * parObject)
{
	int posX = parObject->Position().MinX<int>();
	int posY = parObject->Position().MinY<int>();
	for (auto & cluster : FReachabilityClusters)
	{
		if (cluster->Contains(posX, posY))
		{
			cluster->AddAgent(parAgent);
			return ;
		}
	}
	assert(false);
}

// --------------------------------------------------------
// TODO
// --------------------------------------------------------
int SMAHeat::GetDistanceBetween(Agent const * agent1, Agent const * agent2)
{
	int dist = std::numeric_limits<int>::min();
	ReachabilityCluster * initialCluster = NULL, * destCluster = NULL;
	for (auto const & cluster : FReachabilityClusters)
	{
		if (cluster->Contains(agent1))
			initialCluster = cluster;
		if (cluster->Contains(agent2))
			destCluster = cluster;
	}
	assert(initialCluster && destCluster);
	std::list<Link const *> initialLinks;
	std::list<Link const *> destLinks;
	
	for (auto const & link : initialCluster->GetLinks())
	{
		for (auto const & currentAgent : link->ReachableAgents)
		{
			if (currentAgent.agent == agent1)
			{
				initialLinks.push_back(link);
				break;
			}
		}
	}
	for (auto const & link : destCluster->GetLinks())
	{
		for (auto const & currentAgent : link->ReachableAgents)
		{
			if (currentAgent.agent == agent2)
			{
				destLinks.push_back(link);
				break;
			}
		}
	}

	if (!(initialLinks.size() && destLinks.size()))
		return -1;
	FLinkPathFinder.SetDestinations(destLinks);
	assert(FLinkPathFinder.HasARechableTarget());
	for (auto initialPos : initialLinks)
	{
		FLinkPathFinder.SetInitialPos(initialPos);
		if (FLinkPathFinder.ComputePath()>0)
		{
			dist = FLinkPathFinder.GetDist();
			break;
		}
	}
	// TODO preciser une position initial et un set de destination au path find
	return dist;
}

