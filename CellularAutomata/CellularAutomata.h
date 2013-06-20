#pragma once

#include <set>
#include <utility>

#include <SFML\Graphics.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "CoordConverter.h"

class CellularAutomata
{
	typedef std::set<std::pair<int, int>> CellToUpdateList;
	typedef std::pair<int, int> Size;
public:
	CellularAutomata();
	void Init(int parX, int parY, float parDefaultValue);

	float const * operator[](int parY) const;

	void UpdateCell(int parX, int parY, float parValue);
	void Update();

	void Draw(sf::RenderWindow& app) const;
	CoordConverter const & GetCoordConverter() const { return FCoordConverter; }
	float GetAverageTemp() const { return FAverageTemp; }
	float GetDeltaTemp() const { return FDeltaTemp; }
	void SetViscosity(float parValue) { FViscosity = parValue; }
	// -----------------------------------------------------------
	// SaveLoad
	// -----------------------------------------------------------
	friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
		ar & FAverageTemp;
		ar & FDeltaTemp;
		ar & FCoordConverter;
		ar & FViscosity;

		for (int y = 0; y <= FCoordConverter.GetY(); ++y)
		{
			for (int x = 0; x <= FCoordConverter.GetX(); ++x)
			{	
				ar & FCelluls[y][x];
				ar & FPreviousCelluls[y][x];
			}
		}
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
		ar & FAverageTemp;
		ar & FDeltaTemp;
		ar & FCoordConverter;
		ar & FViscosity;

		FCelluls = new float*[FCoordConverter.GetY() + 1]();
		FPreviousCelluls = new float*[FCoordConverter.GetY() + 1]();
		for (int i = 0; i <= FCoordConverter.GetY(); ++i)
		{
			FPreviousCelluls[i] = new float[FCoordConverter.GetX() + 1]();
			FCelluls[i] = new float[FCoordConverter.GetX() + 1];
			for (int j = 0; j <= FCoordConverter.GetX(); ++j)
			{
				ar & FCelluls[i][j];
				ar & FPreviousCelluls[i][j];
			}
		}
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
protected:
private:
	float FViscosity;
	float FAverageTemp;
	float FDeltaTemp;
	float ** FPreviousCelluls;
	float ** FCelluls;
	CellToUpdateList FCellsToUpdate;

	CoordConverter FCoordConverter;

	
	// ------------- SFML ------------------------
	sf::Shape* FTemperatureShapes[11];
};
