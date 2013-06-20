#pragma once

#include "BaseObject.h"

class Probe : public BaseObject
{
public:
	Probe(void);
	~Probe(void);

	virtual BaseObject * Clone() const;
	virtual void Draw(sf::RenderWindow & app) const;
	virtual void Update(World * parWorld);

	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BaseObject);
    }
private:
	float FTemp;
	sf::Text FText;
	sf::Font FFont;
};

