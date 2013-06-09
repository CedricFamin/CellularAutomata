#include "ObjectFactory.h"


ObjectFactory::ObjectFactory(void)
{
}

void ObjectFactory::RegisterObject(BaseObject const * parObject)
{
	this->FModels[parObject->Identifier()] =  parObject;
}

BaseObject * ObjectFactory::CreateObject(std::string const & parIdentifier) const
{
	ModelsContainerType::const_iterator it = this->FModels.find(parIdentifier);
	if (it == this->FModels.cend())
		return NULL;
	return it->second->Clone();
}
