#include <sstream>

#include "BaseObject.h"


BaseObject::BaseObject(std::string const & parIdentitier)
	: FIdentifier(parIdentitier)
{
}


BaseObject::~BaseObject(void)
{
}

void BaseObject::Init(ParamList const & parParams)
{
	float minX, minY, maxX, maxY;
	std::cout << "--------- BaseObject --------- " << std::endl;
	std::istringstream(parParams[2]) >> minX;
	std::istringstream(parParams[3]) >> minY;
	std::istringstream(parParams[4]) >> maxX;
	std::istringstream(parParams[5]) >> maxY;
	std::cout << "Position((" << minX << ", " << maxX << "), (" << minY << ", " << maxY << "))" << std::endl;
	FPosition.SetPosition(minX, maxX, minY, maxY);
}