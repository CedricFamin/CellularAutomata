#pragma once
#include "BaseObject.h"

class BaseWallObject : public BaseObject
{
public:
	BaseWallObject(std::string const & parIdentifier, std::string const & parTextureName);
	virtual ~BaseWallObject(void);

	virtual void Draw(sf::RenderWindow & app) const;
	virtual void Update(World * parWorld);
	virtual void Init(ParamList const & parParams) override;
	void InitShape();

	friend class boost::serialization::access;
	template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BaseObject);
		ar & FTempToGet;
		ar & FWallTemp;
    }
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BaseObject);
		ar & FTempToGet;
		ar & FWallTemp;
		InitShape();
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

private:
	sf::RectangleShape FShape;
	sf::Texture FTexture;
	float FTempToGet;
	float FWallTemp;
	float FBaseWallTemp;
};

BOOST_CLASS_EXPORT_KEY(BaseWallObject);
