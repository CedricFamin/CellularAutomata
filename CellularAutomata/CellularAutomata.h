#pragma once

#include <set>
#include <utility>

#include <SFML\Graphics.hpp>

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
