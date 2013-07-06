#pragma once

#include <SFML/Graphics.hpp>

class World;

class Interface
{
public:
	Interface(void);
	~Interface(void);

	void Draw(sf::RenderWindow& app, World const & parWorld) const;
	bool OnClick(World * parWorld, int parX, int parY);
	int TemperatureSelected() const { return FTemperatureSelected; }
	void ToggleAutoMode() { FAuto = !FAuto; }
	bool AutoMode() { return FAuto; }
private:
	bool FAuto;
	int FTemperatureSelected;
	sf::Font FDefaultFont;
};

