#include "WallObject.h"

#include "World.h"

BOOST_CLASS_EXPORT_IMPLEMENT(WallObject)

WallObject::WallObject(void)
: BaseObject("wall")
{
	FLayer = 0;
}


WallObject::~WallObject(void)
{
}

void WallObject::Draw(sf::RenderWindow & app) const
{
	sf::Vector2f size(FPosition.maxX - FPosition.minX, FPosition.maxY - FPosition.minY);
	sf::RectangleShape shape(size);

	shape.setPosition(FPosition.minX, FPosition.minY);
	sf::Texture texture;
	texture.loadFromFile("../Tiles/wall0.png");
	texture.setRepeated(true);
	shape.setTexture(&texture);
	sf::IntRect textureRect(sf::Vector2i(FPosition.minX, FPosition.minY), sf::Vector2i(FPosition.maxX - FPosition.minX, FPosition.maxY - FPosition.minY));
	shape.setTextureRect(textureRect);
	
	app.draw(shape);
}

BaseObject * WallObject::Clone() const
{
	return new WallObject();
}

void WallObject::Update(World * parWorld)
{
	// La chaleur de passe pas les mur, on set la temperature invalide
	for (int x = FPosition.minX; x < FPosition.maxX; ++x)
	{
		for (int y = FPosition.minY; y < FPosition.maxY; ++y)
		{
			parWorld->GetCelluls().UpdateCell(x, y, -1.0f);
		}
	}
}