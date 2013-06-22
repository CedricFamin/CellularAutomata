#include <list>

#include "SMAHeat.h"

#include "World.h"

template<typename type = int>
struct Point { type x; type y; };

namespace {
	static const int CLUSTER_SIZE = 10;
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
		rect.setPosition(link.x * 10.0f, link.y * 10.0f);
		rect.setFillColor(sf::Color::Blue);
		app.draw(rect);
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
			c1->AddLink(Link(c2, x - 1, begin + middle));
			c2->AddLink(Link(c1, x, begin + middle));
			begin = -1;
		}
		++left;
	}
	if (begin >= 0)
	{
		int middle = (left - begin) / 2;
		c1->AddLink(Link(c2, x - 1, begin + middle));
		c2->AddLink(Link(c1, x, begin + middle));
		begin = -1;
	}
	return true;
}

// --------------------------------------------------------
// --------------------------------------------------------
bool CreateLinkBot(CellularAutomata const & celluls, VisionCluster * c1, VisionCluster * c2, int left, int right, int y)
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
			c1->AddLink(Link(c2, begin + middle, y - 1));
			c2->AddLink(Link(c1, begin + middle, y));
			begin = -1;
		}
		++left;
	}
	if (begin >= 0)
	{
		int middle = (left - begin) / 2;
		c1->AddLink(Link(c2, begin + middle, y - 1));
		c2->AddLink(Link(c1, begin + middle, y));
		begin = -1;
	}
	return true;
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::BuildVisionCache(World const & parWorld)
{
	CellularAutomata const & celluls = parWorld.GetCelluls();
	CoordConverter const & mapSize = celluls.GetCoordConverter();

	for (unsigned int i = 0; i < FVisionClusters.size(); ++i)
	{
		VisionCluster * currentCluster = FVisionClusters[i];
		
		std::list<std::pair<int, int>> posToCheck;
		// bot
		{
			unsigned int otherClusterIndex = i + 0 + 1 * mapSize.GetX() / CLUSTER_SIZE;
			if (otherClusterIndex >= 0 && otherClusterIndex < FVisionClusters.size())
			{
				VisionCluster * oCluster = FVisionClusters[otherClusterIndex];

				if (currentCluster->MaxX() >= mapSize.GetX()) continue;
				CreateLinkBot(celluls, currentCluster, oCluster, currentCluster->MinX(), currentCluster->MinX() + CLUSTER_SIZE, currentCluster->MaxY());
			}
		}

		// right
		{
			unsigned int otherClusterIndex = i + 1 + 0 * mapSize.GetX() / CLUSTER_SIZE;
			if (otherClusterIndex >= 0 && otherClusterIndex < FVisionClusters.size())
			{
				VisionCluster * oCluster = FVisionClusters[otherClusterIndex];

				if (currentCluster->MaxX() >= mapSize.GetX()) continue;
				CreateLinkRight(celluls, currentCluster, oCluster, currentCluster->MinY(), currentCluster->MinY() + CLUSTER_SIZE, currentCluster->MaxX());
			}
		}
	}
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
}

// --------------------------------------------------------
// --------------------------------------------------------
void SMAHeat::CreateAgent(HeatObject & parHeat)
{
	FDistributorAgent.push_back(new DistributorAgent(this, parHeat));
	RegisterInVisionCache(FDistributorAgent.back(), &parHeat);
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
			cluster->AddAgent(FDistributorAgent.back());
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

	return dist;
}

