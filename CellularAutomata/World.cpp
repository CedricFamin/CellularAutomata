#include <fstream>
#include <sstream>
#include <regex>

#include <boost/filesystem.hpp>

#include "World.h"

#include "AllObject.h"

// --------------------------------------------------------
// World
// --------------------------------------------------------
World::World(ObjectFactory const & parFactory)
: FFactory(parFactory)
, FShowDebug(false)
, FNeedBuildVisionCache(true)
, FWorldSize(0, 0)
, FTickNb(0)
, FTickSize(.2f)
, FPause(false)
, FInitialWorldTemperature(0)
{
}


// --------------------------------------------------------
// --------------------------------------------------------
World::~World(void)
{
}

// --------------------------------------------------------
// --------------------------------------------------------
void World::Draw(sf::RenderWindow& app) const
{
	// draw background 
	{
		sf::Vector2f size((float)(FWorldSize.first), (float)(FWorldSize.second));
		size *= Config::CellulSize;
		sf::RectangleShape shape(size);
		shape.setPosition(sf::Vector2f(0.0f, 0.0f));
		sf::Texture texture;
		texture.loadFromFile("../Tiles/sol.png");
		texture.setRepeated(true);
		shape.setTexture(&texture);
		sf::IntRect textureRect(sf::Vector2i(0, 0), sf::Vector2i(FWorldSize.first * Config::iCellulSize, FWorldSize.second * Config::iCellulSize));
		shape.setTextureRect(textureRect);
		app.draw(shape);
	}

	// draw object first
	for (MapType const & map : FMaps)
	{
		for (BaseObject * object : map)
		{
			object->Draw(app);
		}
	}

	FCellularAutomata.Draw(app);
	if (FShowDebug)
		FSMAHeat.Draw(app);
}

// --------------------------------------------------------
// --------------------------------------------------------
void World::DrawInterface(sf::RenderWindow& app) const
{
	FInterface.Draw(app, *this);
}

// --------------------------------------------------------
// --------------------------------------------------------
bool World::InitWorldParams(ParamList const & parParams)
{
	std::cout << "----------------------------" << std::endl;
	std::cout << "World Params" << std::endl;
	std::istringstream (parParams[1]) >> FWorldSize.first;
	std::istringstream (parParams[2]) >> FWorldSize.second;
	std::istringstream (parParams[3]) >> FInitialWorldTemperature;
	std::cout << "Size(" << FWorldSize.first << " ," << FWorldSize.second << ")" << std::endl;
	std::cout << "Initial temperature " << FInitialWorldTemperature << std::endl;
	std::cout << "----------------------------" << std::endl << std::endl;
	FCellularAutomata.Init(FWorldSize.first, FWorldSize.second, FInitialWorldTemperature);
	FSMAHeat.Init(*this);
	return true;
}

// --------------------------------------------------------
// --------------------------------------------------------
bool World::SetVisicosity(ParamList const & parParams)
{
	float viscosity = 0.0f;
	std::istringstream (parParams[1]) >> viscosity;
	FCellularAutomata.SetViscosity(viscosity);
	return true;
}

// --------------------------------------------------------
// --------------------------------------------------------
bool World::CreateObject(ParamList const & parParams)
{
	std::cout << " --------- Create Object" << std::endl;

	BaseObject * newObject = FFactory.CreateObject(parParams[1]);
	if (newObject)
	{
		newObject->Init(parParams);
		AddObject(newObject);
	}
	else
	{
		std::cout << "Warning: Unrecognized object type " << parParams[1] << std::endl;
	}
	return true;
}

// --------------------------------------------------------
// --------------------------------------------------------
void World::AddObject(BaseObject * parObject)
{
	if (FMaps.size() < parObject->GetLevelLayer() + 1)
		FMaps.resize(parObject->GetLevelLayer() + 1);
	FMaps[parObject->GetLevelLayer()].push_back(parObject);

	parObject->CreateAgent(FSMAHeat);
}

// --------------------------------------------------------
// --------------------------------------------------------
bool World::LoadWorld(std::string const & parFilename)
{
	boost::filesystem::path const p(parFilename);
	FMapName = p.stem().string();

	typedef std::map<std::string, bool(World::*)(ParamList const &)>  MapOperationType;
	MapOperationType operation;
	{
		operation["world"] = &World::InitWorldParams;
		operation["object"] = &World::CreateObject;
		operation["viscosity"] = &World::SetVisicosity;
	}

	std::ifstream fileStream;
	fileStream.open(parFilename.c_str());
	std::smatch m;
	std::regex e ("([a-z|A-Z|0-9]+) ?");

	

	if (!fileStream.is_open())
	{
		std::cout << "Error: Cannot open file " << parFilename << std::endl;
		return false;
	}

	char line[512];
	while (fileStream.good())
	{
		fileStream.getline(line, 512);
		std::string s(line);
		std::vector<std::string> params;

		while (std::regex_search(s,m,e))
		{
			params.push_back(m[1]);
			s = m.suffix().str();
		}

		if (params.size())
		{
			MapOperationType::const_iterator itOp = operation.find(params[0]);
			if (itOp != operation.end())
			{
				if (!(this->*(itOp->second))(params))
					return false;
			}
			else 
			{
				std::cout << "Warning: Unrocognized token " << params[0] << std::endl;
			}
		}
	}
	return true;
}

// --------------------------------------------------------
// --------------------------------------------------------
void World::AfterLoad()
{
	//FSMAHeat.BuildVisionCache(*this);
}

// --------------------------------------------------------
// --------------------------------------------------------
void World::OnTick()
{
	++FTickNb;
	// Update Object
	for (MapType const & map : FMaps)
	{
		for (BaseObject * object : map)
		{
			object->Update(this);
		}
	}

	FCellularAutomata.Update();
	if (FNeedBuildVisionCache)
	{
		FSMAHeat.BuildVisionCache(*this);
		FNeedBuildVisionCache = false;
	}
	if (FInterface.AutoMode())
		FSMAHeat.Update(*this);
}

// --------------------------------------------------------
// --------------------------------------------------------
void World::OnClick(sf::RenderTarget const & parApp, int parX, int parY)
{
	if (FInterface.OnClick(this, parX, parY))
		return ;

	sf::Vector2f realPosition = parApp.mapPixelToCoords(sf::Vector2i(parX, parY), parApp.getView());
	realPosition /= Config::CellulSize;
	int x = (int)realPosition.x;
	int y = (int)realPosition.y;
	for (MapType const & map : FMaps)
	{
		for (BaseObject * object : map)
		{
			if (object->OnClick(this, x, y))
				return ;
		}
	}

	float temperature = (float)FInterface.TemperatureSelected();
	
	
	if (x >= 0 && x < FCellularAutomata.GetSize().x && y >= 0 && y < FCellularAutomata.GetSize().y)
		FCellularAutomata[y][x].Temp = temperature;
}
