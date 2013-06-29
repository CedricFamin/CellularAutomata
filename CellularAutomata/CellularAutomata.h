#pragma once

#include <set>
#include <utility>

#include <SFML\Graphics.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "Utils.h"

struct Cellul
{
	Cellul() : IsWall(false), Temp(0.0f) { }
	bool IsWall;
	float Temp;

	// -----------------------------------------------------------
	// SaveLoad
	// -----------------------------------------------------------
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & IsWall;
		ar & Temp;
    }
};

class CellularAutomata
{
	typedef std::set<std::pair<int, int>> CellToUpdateList;
	typedef std::pair<int, int> Size;
	typedef std::vector<std::vector<Cellul>> GridType;
public:
	CellularAutomata();
	void Init(int parX, int parY, float parDefaultValue);

	std::vector<Cellul> const & operator[](int parY) const;
	std::vector<Cellul> & operator[](int parY);

	void Update();

	void Draw(sf::RenderWindow& app) const;
	PointI const & GetSize() const { return FSize; }
	float GetAverageTemp() const { return FAverageTemp; }
	float GetDeltaTemp() const { return FDeltaTemp; }
	void SetViscosity(float parValue) { FViscosity = parValue; }
	// -----------------------------------------------------------
	// SaveLoad
	// -----------------------------------------------------------
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & FAverageTemp;
		ar & FDeltaTemp;
		ar & FViscosity;
		ar & FCelluls;
		ar & FPreviousCelluls;
		ar & FSize;
    }
protected:
private:
	float FViscosity;
	float FAverageTemp;
	float FDeltaTemp;
	GridType FPreviousCelluls;
	GridType FCelluls;
	CellToUpdateList FCellsToUpdate;

	PointI FSize;

	
	// ------------- SFML ------------------------
	sf::Shape* FTemperatureShapes[11];
};
