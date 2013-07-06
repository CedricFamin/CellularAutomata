#include "StoneWallObject.h"

BOOST_CLASS_EXPORT_IMPLEMENT(StoneWallObject)

StoneWallObject::StoneWallObject()
	: BaseWallObject("stonewall", "../Tiles/wall-stone.png")
{
}

BaseObject * StoneWallObject::Clone() const
{
	return new StoneWallObject();
}