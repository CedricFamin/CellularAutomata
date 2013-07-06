#pragma once

#include "BaseWallObject.h"

class StoneWallObject : public BaseWallObject
{
public:
	StoneWallObject();
	virtual BaseObject * Clone() const;
    
    friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BaseWallObject);
    }
};

BOOST_CLASS_EXPORT_KEY(StoneWallObject);
