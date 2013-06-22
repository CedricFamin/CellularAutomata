#include "SMAHeat.h"

#include "World.h"

template<typename type = int>
struct Point { type x; type y; };

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
void SMAHeat::BuildVisionCache(World const & parWorld)
{
	CellularAutomata const & celluls = parWorld.GetCelluls();
	CoordConverter const & mapSize = celluls.GetCoordConverter();

	for (unsigned int i = 0; i < FVisionClusters.size(); ++i)
	{
		VisionCluster * currentCluster = FVisionClusters[i];
		int indexCluster[][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
		
		for (unsigned int j = 0; j < 4; ++j)
		{

			int otherClusterIndex = i + indexCluster[j][0] + indexCluster[j][1] * mapSize.GetX() / 10;
			if (otherClusterIndex >= 0 && otherClusterIndex < FVisionClusters.size())
			{
				VisionCluster * oCluster = FVisionClusters[otherClusterIndex];

				int x = (currentCluster->MaxX() - currentCluster->MinX()) / 2 + currentCluster->MinX();
				int y = currentCluster->MaxY() - 1;
				if (parWorld.GetCelluls()[y][x] >= 0)
				{
					Link link;
					link.x = x;
					link.y = y;
					link.Cluster = oCluster;
					currentCluster->AddLink(link);
				}
				break;
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

