#pragma once

#include <iostream>
#include <vector>

#include <SFML\Graphics.hpp>

class World;

// Class de base pour les objet du monde

class BaseObject
{
	typedef std::vector<std::string> ParamList;
public:
	struct ObjectPosition
	{
		int minX;
		int maxX;
		int minY;
		int maxY;
	};

	BaseObject(std::string const & parIdentitier);
	virtual ~BaseObject(void);

	std::string const & Identifier(void) const { return FIdentifier; }

	virtual BaseObject * Clone() const = 0;
	virtual void Init(ParamList const & parParams);
	unsigned int GetLevelLayer(void) const { return FLayer; }
	virtual void Draw(sf::RenderWindow & app) const = 0;
	virtual void Update(World * parWorld) {}
	virtual void OnClick(World * parWorld, int parX, int parY) {}
protected:
	int FLayer;
	ObjectPosition FPosition;
	std::string FIdentifier;

};

