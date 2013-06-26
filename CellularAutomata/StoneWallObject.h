#pragma once

#include "BaseWallObject.h"

class StoneWallObject : public BaseWallObject
{
public:
	StoneWallObject();
	virtual BaseObject * Clone() const;
};

