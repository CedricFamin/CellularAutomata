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
, FTickNb(0)
{
	FDefaultFont.loadFromFile("../font/arial.ttf");
}


World::~World(void)
{
}

void World::Draw(sf::RenderWindow& app) const
{
	// draw background 
	{
		sf::Vector2f size((float)(FWorldSize.first), (float)(FWorldSize.second));
		sf::RectangleShape shape(size);
		shape.setPosition(sf::Vector2f(0.0f, 0.0f));
		sf::Texture texture;
		texture.loadFromFile("../Tiles/sol.png");
		texture.setRepeated(true);
		shape.setTexture(&texture);
		sf::IntRect textureRect(sf::Vector2i(0, 0), sf::Vector2i(FWorldSize.first, FWorldSize.second));
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
}

void World::DrawInterface(sf::RenderWindow& app) const
{
	{ 
		// background 
		sf::Vector2f size(app.getSize().x, 45);
		sf::RectangleShape shape(size);
		shape.setPosition(0, 0);
		shape.setFillColor(sf::Color(0, 0, 0, 155));
		shape.setOutlineColor(sf::Color(200, 200, 200, 200));
		shape.setOutlineThickness(-3.0f);
		app.draw(shape);
	}

	// stat
	{ // tick
		std::ostringstream strText;
		strText << "TickSize : " << FTickSize;
		sf::Text txt(strText.str(), FDefaultFont, 10);
		txt.setPosition(10, 10);
		app.draw(txt);
	}
	{ // average temp
		std::ostringstream strText;
		strText << "Average Temp : " << FCellularAutomata.GetAverageTemp();
		sf::Text txt(strText.str(), FDefaultFont, 10);
		txt.setPosition(10, 25);
		app.draw(txt);
	}
	{ // average temp
		std::ostringstream strText;
		strText << "Delta temp : " << FCellularAutomata.GetDeltaTemp();
		sf::Text txt(strText.str(), FDefaultFont, 10);
		txt.setPosition(150, 25);
		app.draw(txt);
	}
	{ // Tick
		std::ostringstream strText;
		strText << "Tick : " << FTickNb;
		sf::Text txt(strText.str(), FDefaultFont, 10);
		txt.setPosition(150, 10);
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