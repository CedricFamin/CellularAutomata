#include <list>
#include <queue>

#include "SMAHeat.h"

#include "World.h"
#include "ReachablePathFind.h"

template<typename type = int>
struct Point { type x; type y; };

namespace {
	static const int CLUSTER_SIZE = 10;
	static const float FCLUSTER_SIZE = static_cast<float>(CLUSTER_SIZE);
}

// --------------------------------------------------------
// VisionCluster
// --------------------------------------------------------
VisionCluster::VisionCluster(int parMinX, int parMaxX, int parMinY, int parMaxY)
	: FMinX(parMinX)
	, FMaxX(parMaxX)
	, FMinY(parMinY)
	, FMaxY(parMaxY)
{
}

void VisionCluster::Draw(sf::RenderWindow& app) const
{
	sf::Vector2f size(10.0f, 10.0f);
	for (auto const & link : FLinks)
	{
		sf::RectangleShape rect(size);
		rect.setPosition(link->x * FCLUSTER_SIZE, link->y * FCLUSTER_SIZE);
		rect.setFillColor(sf::Color::Blue);
		app.draw(rect);

		for (auto const & connectedLink : link->ReachableLinks)
		{
			sf::VertexArray lines(sf::LinesStrip, 2);
			lines[0].position = sf::Vector2f(link->x * FCLUSTER_SIZE, link->y * FCLUSTER_SIZE);
			lines[1].position = sf::Vector2f(connectedLink->x * FCLUSTER_SIZE, connectedLink->y * FCLUSTER_SIZE);
			app.draw(lines);
		}

		for (auto const & connectedAgent : link->ReachableAgents)
		{
			sf::VertexArray lines(sf::LinesStrip, 2);
			lines[0].position = sf::Vector2f(link->x * FCLUSTER_SIZE, link->y * FCLUSTER_SIZE);
			lines[1].position = sf::Vector2f(connectedAgent->X() * FCLUSTER_SIZE, connectedAgent->Y() * FCLUSTER_SIZE);
			app.draw(lines);
		}
	}
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
	CoordConverter const & mapSize = celluls.GetCoordConverter();

	int nbCluster = mapSize.GetX() * mapSize.GetY() / 100;

	for (int i = 0; i < nbCluster; ++i)
	{
		int minPosX = (i % (mapSize.GetX() / 10)) * 10;
		int minPosY = (i / (mapSize.GetX() / 10)) * 10;
		VisionCluster * cluster = new VisionCluster(minPosX, minPosX + 10, minPosY, minPosY + 10);
		FVisionClusters.push_back(cluster);
	}
}

// --------------------------------------------------------
// --------------------------------------------------------
bool CreateLinkRight(CellularAutomata const & celluls, VisionCluster * c1, VisionCluster * c2, int left, int right, int x)
{
	
	int begin = -1;
	while (left < right)
	{
		if (celluls[left][x] != -1 && celluls[left][x - 1] != -1)
		{
			if (begin == -1)
				begin = left;
		}
		else if (begin >= 0)
		{
			int middle = (left - begin) / 2;
			Link * cLink1 = new Link(c2, x - 1, begin + middle);
			Link * cLink2 = new Link(c1, x, begin + middle);
			cLink1->ReachableLinks.push_back(cLink2);
			cLink2->ReachableLinks.push_back(cLink1);
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
		cLink1->ReachableLinks.push_back(cLink2);
		cLink2->ReachableLinks.push_back(cLink1);
		c1->AddLink(cLink1);
		c2->AddLink(cLink2);
		begin = -1;
	}
	return true;
}

// --------------------------------------------------------
// --------------------------------------------------------
void CreateLinkBot(CellularAutomata const & celluls, VisionCluster * c1, VisionCluster * c2, int left, int right, int y)
{
	
	int begin = -1;
	while (left < right)
	{
		if (celluls[y][left] != -1 && celluls[y - 1][left] != -1)
		{
			if (begin == -1)
				begin = left;
		}
		else if (begin >= 0)
		{
			int middle = (left - begin) / 2;
			Link * cLink1 = new Link(c2, begin + middle, y - 1);
			Link * cLink2 = new Link(c1, begin + middle, y);
			cLink1->ReachableLinks.push_back(cLink2);
			cLink2->ReachableLinks.push_back(cLink1);
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
		cLink1->ReachableLinks.push_back(cLink2);
		cLink2->ReachableLinks.push_back(cLink1);
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
	CoordConverter const & mapSize = celluls.GetCoordConverter();

	std::cout << "build Cluster external link" << std::endl;
	for (unsigned int i = 0; i < FVisionClusters.size(); ++i)
	{
		VisionCluster * currentCluster = FVisionClusters[i];
		// bot
		{
			unsigned int otherClusterIndex = i + 0 + 1 * mapSize.GetX() / CLUSTER_SIZE;
			if (otherClusterIndex >= 0 && otherClusterIndex < FVisionClusters.size())
			{
				VisionCluster * oCluster = FVisionClusters[otherClusterIndex];
				if (currentCluster->MaxX() >= mapSize.GetX()) continue;
				CreateLinkBot(celluls, currentCluster, oCluster, currentCluster->MinX(), 
					currentCluster->MinX() + CLUSTER_SIZE, currentCluster->MaxY());
			}
		}

		// right
		{
			unsigned int otherClusterIndex = i + 1 + 0 * mapSize.GetX() / CLUSTER_SIZE;
			if (otherClusterIndex >= 0 && otherClusterIndex < FVisionClusters.size())
			{
				VisionCluster * oCluster = FVisionClusters[otherClusterIndex];
				if (currentCluster->MaxX() >= mapSize.GetX()) continue;
				CreateLinkRight(celluls, currentCluster, oCluster, currentCluster->MinY(), 
					currentCluster->MinY() + CLUSTER_SIZE, currentCluster->MaxX());
			}
		}
	}

	std::cout << "build Cluster internal link <=> link" << std::endl;

	for (VisionCluster * cluster : FVisionClusters)
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
					if (pathFind.ComputePath())
					{
						link->ReachableLinks.push_back(oLink);
					}
				}
			}
		}
	}
	std::cout << "build Cluster: End" << std::endl;
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::BuildVisionCache(World const & parWorld, Agent* parAgent)
{
	std::cout << "build Cluster internal link <=> Agent" << std::endl;
	VisionCluster * cluster = NULL;
	for (auto const & currentCluster : FVisionClusters)
	{
		if (currentCluster->Contains(parAgent))
			cluster = currentCluster;
	}

	ReachablePathFind pathFind(&parWorld.GetCelluls(), cluster);
	// Reachabilite entre les different lien vers l'exterieur
	for (auto& link : cluster->GetLinks())
	{
		for (auto& link : cluster->GetLinks())
		{
			Position p1 = { link->x, link->y };
			Position p2 = { parAgent->X(), parAgent->Y() };
			pathFind.Init(p1, p2);
			if (pathFind.ComputePath())
			{
				link->ReachableAgents.push_back(parAgent);
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
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::Draw(sf::RenderWindow& app) const
{
	sf::Vector2f clusterSize(100.0f, 100.0f);

	for (auto & cluster : FVisionClusters)
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
	FDistributorAgent.push_back(new DistributorAgent(this, parHeat));
	RegisterInVisionCache(FDistributorAgent.back(), &parHeat);
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
	int posX = parObject->Position().MinX<int>() / 10;
	int posY = parObject->Position().MinY<int>() / 10;
	for (auto & cluster : FVisionClusters)
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
// --------------------------------------------------------
int SMAHeat::GetDistanceBetween(Agent const * agent1, Agent const * agent2)
{
	int dist = std::numeric_limits<int>::min();
	VisionCluster * initialCluster = NULL, * destCluster = NULL;
	for (auto const & cluster : FVisionClusters)
	{
		if (cluster->Contains(agent1))
			initialCluster = cluster;
		if (cluster->Contains(agent2))
			destCluster = cluster;
	}
	assert(initialCluster && destCluster);
	Link * intialLink = NULL, * destLink = NULL;
	for (auto const & link : initialCluster->GetLinks())
	{
		if (std::find(link->ReachableAgents.begin(), link->ReachableAgents.end(), agent1) != link->ReachableAgents.end())
		{
			intialLink = link;
			break;
		}
	}
	for (auto const & link : destCluster->GetLinks())
	{
		if (std::find(link->ReachableAgents.begin(), link->ReachableAgents.end(), agent2) != link->ReachableAgents.end())
		{
			destLink = link;
			break;
		}
	}
	if (!(intialLink && destLink))
		return -1;

	std::list<Link const *> openedLink;
	std::unordered_set<Link const *> closedLink;
	openedLink.push_back(intialLink);
	while (openedLink.size())
	{
		Link const * currentLink = openedLink.front();
		openedLink.pop_front();
		if (currentLink == destLink)
			return 1;
		for (auto edge : currentLink->ReachableLinks)
		{
			if (std::find(openedLink.begin(), openedLink.end(), edge) != openedLink.end())
				continue;
			if (closedLink.find(edge) != closedLink.end())
				continue;
			openedLink.push_back(edge);
		}
		closedLink.insert(currentLink);
	}
	return dist;
}

