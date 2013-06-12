#pragma once

#include <set>
#include <utility>

#include <SFML\Graphics.hpp>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

class CellularCoordConverter
{
public:
	void Init(int parRealX, int parRealY);

	std::pair<int, int> MapCoordToCellulCoord(int parRealX, int parRealY) const;
	std::pair<int, int> CellulCoordToMapCoord(int parX, int parY) const;

	int GetRealX() const { return FRealSizeX; }
	int GetRealY() const { return FRealSizeY; }
	int GetX() const { return FSizeX; }
	int GetY() const { return FSizeY; }

	// -----------------------------------------------------------
	// SaveLoad
	// -----------------------------------------------------------
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & FRealSizeX;
		ar & FRealSizeY;
		ar & FSizeX;
		ar & FSizeY;
    }
protected:
private:
	int FRealSizeX;
	int FRealSizeY;
	int FSizeX;
	int FSizeY;
};

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
	CellularCoordConverter const & CoordConverter() const { return FCoordConverter; }
	float GetAverageTemp() const { return FAverageTemp; }
	float GetDeltaTemp() const { return FDeltaTemp; }

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
	float FAverageTemp;
	float FDeltaTemp;
	float ** FPreviousCelluls;
	float ** FCelluls;
	CellToUpdateList FCellsToUpdate;

	CellularCoordConverter FCoordConverter;

	
	// ------------- SFML ------------------------
	sf::Shape* FTemperatureShapes[11];
};
