#pragma once
#include "BaseObject.h"
#include "BaseWallObject.h"

class WallObject : public BaseWallObject
{
public:
	WallObject();
	virtual BaseObject * Clone() const;
};

