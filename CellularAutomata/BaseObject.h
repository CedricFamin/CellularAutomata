#pragma once

#include <iostream>
#include <vector>

#include <SFML\Graphics.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class World;

// Class de base pour les objet du monde

class BaseObject
{
	typedef std::vector<std::string> ParamList;
public:
	struct ObjectPosition
	{
		int minX;
		int maxX;
		int minY;
		int maxY;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & minX;
			ar & maxX;
			ar & minY;
			ar & maxY;
		}
	};

	BaseObject(std::string const & parIdentitier);
	virtual ~BaseObject(void);

	std::string const & Identifier(void) const { return FIdentifier; }

	virtual BaseObject * Clone() const = 0;
	virtual void Init(ParamList const & parParams);
	unsigned int GetLevelLayer(void) const { return FLayer; }
	virtual void Draw(sf::RenderWindow & app) const = 0;
	virtual void Update(World * parWorld) {}
	virtual void OnClick(World * parWorld, int parX, int parY) {}

	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & FLayer;
		ar & FPosition;
		ar & FIdentifier;
    }

protected:
	int FLayer;
	ObjectPosition FPosition;
	std::string FIdentifier;

};

BOOST_CLASS_EXPORT_KEY(BaseObject);
