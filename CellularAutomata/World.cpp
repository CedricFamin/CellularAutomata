#include <fstream>
#include <regex>
#include <sstream>

#include "boost/filesystem.hpp"

#include "World.h"

#include "AllObject.h"

World::World(ObjectFactory const & parFactory)
: FFactory(parFactory)
, FWorldSize(0, 0)
, FInitialWorldTemperature(0)
, FTickSize(.2f)
, FPause(false)
{
	FDefaultFont.loadFromFile("../font/arial.ttf");
}


World::~World(void)
{
}

void World::Draw(sf::RenderWindow& app) const
{
	// draw object first
	for (MapType const & map : FMaps)
	{
		for (BaseObject * object : map)
		{
			object->Draw(app);
		}
	}

	FCellularAutomata.Draw(app);

	// draw interface
	{ // tick
		std::ostringstream strText;
		strText << "TickSize : " << FTickSize;
		sf::Text txt(strText.str(), FDefaultFont, 10);
		sf::Vector2f pos = app.mapPixelToCoords(sf::Vector2i(10, 10), app.getView());
		txt.setPosition(pos.x, pos.y);
		app.draw(txt);
	}
}

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
	return true;
}

bool World::CreateObject(ParamList const & parParams)
{
	std::cout << " --------- Create Object" << std::endl;

	BaseObject * newObject = FFactory.CreateObject(parParams[1]);
	if (newObject)
	{
		newObject->Init(parParams);
		if (FMaps.size() < newObject->GetLevelLayer() + 1)
			FMaps.resize(newObject->GetLevelLayer() + 1);
		FMaps[newObject->GetLevelLayer()].push_back(newObject);
	}
	else
	{
		std::cout << "Warning: Unrecognized object type " << parParams[1] << std::endl;
	}
	return true;
}

bool World::LoadWorld(std::string const & parFilename)
{
	boost::filesystem::path const p(parFilename);
	FMapName = p.stem().string();

	typedef std::map<std::string, bool(World::*)(ParamList const &)>  MapOperationType;
	MapOperationType operation;
	{
		operation["world"] = &World::InitWorldParams;
		operation["object"] = &World::CreateObject;
	}

	std::ifstream fileStream;
	fileStream.open(parFilename);
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

void World::OnTick()
{
	// Update Object
	for (MapType const & map : FMaps)
	{
		for (BaseObject * object : map)
		{
			object->Update(this);
		}
	}

	FCellularAutomata.Update();
}

void World::OnClick(int parX, int parY)
{
	if (parX < 0 || parY < 0 || parX >= FWorldSize.first || parY >= FWorldSize.second)
		return ;
	for (MapType const & map : FMaps)
	{
		for (BaseObject * object : map)
		{
			object->OnClick(this, parX, parY);
		}
	}
}