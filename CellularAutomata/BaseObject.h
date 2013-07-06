#pragma once

#include <iostream>
#include <vector>


#include <SFML/Graphics.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/export.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "SMAHeat.h"

class World;

struct ObjectPosition
{
public:
	void SetPosition(float parMinX, float parMaxX, float parMinY, float parMaxY)
	{
		minX = parMinX;
		maxX = parMaxX;
		minY = parMinY;
		maxY = parMaxY;
	}

	template<typename type>
	auto MinX() const -> type { return static_cast<type>(minX); }
	template<typename type>
	auto MaxX() const -> type { return static_cast<type>(maxX); }
	template<typename type>
	auto MinY() const -> type { return static_cast<type>(minY); }
	template<typename type>
	auto MaxY() const -> type { return static_cast<type>(maxY); }
	template<typename type>
	auto SizeX() const -> type { return static_cast<type>(maxX - minX); }
	template<typename type>
	auto SizeY() const -> type { return static_cast<type>(maxY - minY); }

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & minX;
		ar & maxX;
		ar & minY;
		ar & maxY;
	}
private:
	float minX;
	float maxX;
	float minY;
	float maxY;
};


// Class de base pour les objet du monde
class BaseObject
{
public:
	typedef std::vector<std::string> ParamList;
	BaseObject(std::string const & parIdentitier);
	virtual ~BaseObject(void);

	std::string const & Identifier(void) const { return FIdentifier; }

	virtual BaseObject * Clone() const = 0;
	virtual void Init(ParamList const & parParams);
	unsigned int GetLevelLayer(void) const { return FLayer; }
	virtual void Draw(sf::RenderWindow & app) const  { };
	virtual void Update(World * parWorld) {}
	virtual bool OnClick(World * parWorld, int parX, int parY) { return false; }
	ObjectPosition const & Position() const { return FPosition; }

	friend class boost::serialization::access;
	template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & FLayer;
        ar & FIdentifier;
		ar & FPosition;
    }

	virtual void CreateAgent(SMAHeat & parSMA)
	{
		parSMA.CreateAgent(*this);
	}
protected:
	int FLayer;
	ObjectPosition FPosition;
	std::string FIdentifier;

};

BOOST_CLASS_EXPORT_KEY(BaseObject);
