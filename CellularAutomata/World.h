#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <unordered_set>
#include <set>

#include <SFML\Graphics.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "ObjectFactory.h"
#include "BaseObject.h"
#include "CellularAutomata.h"

class WorldSizeType
{
public:
	WorldSizeType(int v1, int v2) : first(v1), second(v2) { }
	int first;
	int second;

	friend class boost::serialization::access;
	// ----------------- SAVELOAD ------------------------------
	friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
		ar  & first;
		ar  & second;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar  & first;
		ar  & second;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};


class World
{
	typedef std::vector<BaseObject*> MapType;
	typedef std::vector<std::string> ParamList;
	typedef std::vector<MapType> MapWithTileLevel;
public:
	World(ObjectFactory const & parFactory);
	~World(void);

	bool LoadWorld(std::string const & parFilename);
	int GetX() const { return FWorldSize.first; }
	int GetY() const { return FWorldSize.second; }
	void Draw(sf::RenderWindow& app) const;
	// World logic
	void OnTick();
	void OnClick(int parX, int parY);

	CellularAutomata& GetCelluls() { return FCellularAutomata; }

	// ----------------- SAVELOAD ------------------------------
	friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
		ar  & FWorldSize;
		ar  & FInitialWorldTemperature;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar  & FWorldSize;
		ar  & FInitialWorldTemperature;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

private:
	bool InitWorldParams(ParamList const & parParams);
	bool CreateObject(ParamList const & parParams);
	
	WorldSizeType FWorldSize;
	float FInitialWorldTemperature;

	ObjectFactory const & FFactory;

	MapWithTileLevel FMaps;
	CellularAutomata FCellularAutomata;
};

