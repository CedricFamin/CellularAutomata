#pragma once
#include "BaseObject.h"

class WallObject : public BaseObject
{
public:
	WallObject(void);
	virtual ~WallObject(void);

	virtual void Draw(sf::RenderWindow & app) const;
	virtual BaseObject * Clone() const;
	virtual void Update(World * parWorld);
};

