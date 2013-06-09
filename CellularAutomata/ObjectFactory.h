#pragma once

#include <iostream>
#include <string>
#include <map>

#include "BaseObject.h"

class ObjectFactory
{
	typedef std::map<std::string, BaseObject const *> ModelsContainerType;
public:
	ObjectFactory(void);

	void RegisterObject(BaseObject const * parObject);
	BaseObject * CreateObject(std::string const & parIdentifier) const;

private:
	ModelsContainerType FModels;

};

