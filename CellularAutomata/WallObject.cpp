#include "WallObject.h"

#include "World.h"

BOOST_CLASS_EXPORT_IMPLEMENT(WallObject)

WallObject::WallObject(void)
: BaseWallObject("wall", "../Tiles/wall0.png")
{
	FLayer = 0;
}


BaseObject * WallObject::Clone() const
{
	return new WallObject();
}