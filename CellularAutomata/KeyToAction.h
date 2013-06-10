#pragma once

#include <map>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "World.h"

class KeyToAction
{
public:
	typedef void(*ActionType)(sf::RenderWindow &, sf::View &, World &);

	KeyToAction();

	ActionType operator[](sf::Keyboard::Key const & parKey);
protected:
private:
	std::map<sf::Keyboard::Key, ActionType> FKeyToAction;
};


