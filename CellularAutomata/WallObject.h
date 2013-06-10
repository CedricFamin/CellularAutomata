#pragma once
#include "BaseObject.h"

class WallObject : public BaseObject
{
public:
	WallObject(void);
	virtual ~WallObject(void);

	virtual void Draw(sf::RenderWindow & app) const;
	virtual BaseObject * Clone() const;
	virtual void Update(World * parWorld);

	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BaseObject);
    }
};

BOOST_CLASS_EXPORT_KEY(WallObject);
