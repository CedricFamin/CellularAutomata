#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <set>

#include <SFML/Graphics.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "ObjectFactory.h"
#include "BaseObject.h"
#include "CellularAutomata.h"
#include "Interface.h"
#include "SMAHeat.h"

#include "AllObject.h"

namespace Config
{
	static const float CellulSize = 10.0f;
	static const int iCellulSize = 10;
};

class WorldSizeType
{
public:
	WorldSizeType(int v1, int v2) : first(v1), second(v2) { }
	int first;
	int second;

	// ----------------- SAVELOAD ------------------------------
	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar  & first;
		ar  & second;
    }
};


class World
{
	typedef std::vector<BaseObject*> MapType;
	typedef std::vector<MapType> MapWithTileLevel;
public:
	typedef std::vector<std::string> ParamList;
	World(ObjectFactory const & parFactory);
	~World(void);

	void AfterLoad();
	bool LoadWorld(std::string const & parFilename);
	int GetX() const { return FWorldSize.first; }
	int GetY() const { return FWorldSize.second; }
	void Draw(sf::RenderWindow& app) const;
	void DrawInterface(sf::RenderWindow& app) const;

	// World logic
	void OnTick();
	void OnClick(sf::RenderTarget const & parApp, int parX, int parY);
	float GetTickSize() const { return FTickSize; }
	void SetTickSize(float parTickSize) { FTickSize = parTickSize; }
	unsigned int GetTick() const { return FTickNb; }
	void TogglePause() { FPause = !FPause; }
	bool Pause() const { return FPause; }
	void ToggleAutoMode() { FInterface.ToggleAutoMode(); }
	void ToggleDebug() { FShowDebug = !FShowDebug; }
	void AddObject(BaseObject * parObject);

	CellularAutomata& GetCelluls() { return FCellularAutomata; }
	CellularAutomata const& GetCelluls() const { return FCellularAutomata; }
	std::string const & GetMapName() const {return FMapName; }

	// ----------------- SAVELOAD ------------------------------
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & FWorldSize;
		ar & FInitialWorldTemperature;
		ar & FMapName;
		ar & FMaps;
		ar & FTickNb;
		ar & FCellularAutomata;
    }

private:
	bool InitWorldParams(ParamList const & parParams);
	bool CreateObject(ParamList const & parParams);
	bool SetVisicosity(ParamList const & parParams);
	
	ObjectFactory const & FFactory;

	bool FShowDebug;
	bool FNeedBuildVisionCache;
	std::string FMapName;
	WorldSizeType FWorldSize;
	unsigned int FTickNb;
	float FTickSize;
	bool FPause;
	float FInitialWorldTemperature;


	MapWithTileLevel FMaps;
	CellularAutomata FCellularAutomata;
	Interface FInterface;
	SMAHeat FSMAHeat;

};

