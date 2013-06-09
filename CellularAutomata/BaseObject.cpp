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
	std::cout << "--------- BaseObject --------- " << std::endl;
	std::istringstream(parParams[2]) >> FPosition.minX;
	std::istringstream(parParams[3]) >> FPosition.minY;
	std::istringstream(parParams[4]) >> FPosition.maxX;
	std::istringstream(parParams[5]) >> FPosition.maxY;
	std::cout << "Position((" << FPosition.minX << ", " << FPosition.maxX << "), (" << FPosition.minY << ", " << FPosition.maxY << "))" << std::endl;
}