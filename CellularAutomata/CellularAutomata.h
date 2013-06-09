#pragma once

#include <set>
#include <utility>

#include <SFML\Graphics.hpp>

class CellularAutomata
{
	typedef std::set<std::pair<int, int>> CellToUpdateList;
public:
	CellularAutomata();
	void Init(int parX, int parY, float parDefaultValue);

	float const * operator[](int parY) const;

	void UpdateCell(int parX, int parY, float parValue);
	void Update();

	void Draw(sf::RenderWindow& app) const;
protected:
private:
	int FSizeX;
	int FSizeY;
	float ** FPreviousCelluls;
	float ** FCelluls;
	CellToUpdateList FCellsToUpdate;

	
	// ------------- SFML ------------------------
	sf::Shape* FTemperatureShapes[11];
};
