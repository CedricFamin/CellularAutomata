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
	std::pair<int, int> cellulCoord = parWorld->GetCelluls().CoordConverter().MapCoordToCellulCoord(FPosition.minX, FPosition.minY);
	int maxX = parWorld->GetCelluls().CoordConverter().GetX();
	int maxY = parWorld->GetCelluls().CoordConverter().GetY();
	int baseX = cellulCoord.first;
	int baseY = cellulCoord.second;
	// La chaleur de passe pas les mur, on set la temperature invalide
	for (int x = baseX; x < maxX ; ++x)
	{
		std::pair<int, int> realCoord = parWorld->GetCelluls().CoordConverter().CellulCoordToMapCoord(x, baseY);
		if (realCoord.first >= FPosition.maxX)
			break;
		for (int y = baseY; y < maxY; ++y)
		{
			realCoord = parWorld->GetCelluls().CoordConverter().CellulCoordToMapCoord(x, y);
			if (realCoord.second >= FPosition.maxY)
				break;
			parWorld->GetCelluls().UpdateCell(x, y, -1.0f);
		}
		
	}
}