#include "WallObject.h"

#include "World.h"

WallObject::WallObject(void)
: BaseWallObject("wall", "../Tiles/wall0.png")
{
	FLayer = 0;
}


BaseObject * WallObject::Clone() const
{
	return new WallObject();
}