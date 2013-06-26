#include "StoneWallObject.h"

StoneWallObject::StoneWallObject()
	: BaseWallObject("stonewall", "../Tiles/wall-stone.png")
{
}

BaseObject * StoneWallObject::Clone() const
{
	return new StoneWallObject();
}