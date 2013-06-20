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

void WallObject::Init(ParamList const & parParams)
{
	BaseObject::Init(parParams);
	InitShape();
}

void WallObject::InitShape()
{
	sf::Vector2f size((float)(FPosition.maxX - FPosition.minX), (float)(FPosition.maxY - FPosition.minY));
	FShape.setSize(size);
	FShape.setPosition((float)FPosition.minX, (float)FPosition.minY);
	FTexture.loadFromFile("../Tiles/wall0.png");
	FTexture.setRepeated(true);
	FShape.setTexture(&FTexture);
	sf::IntRect textureRect(sf::Vector2i(0, 0), sf::Vector2i(FPosition.maxX - FPosition.minX, FPosition.maxY - FPosition.minY));
	FShape.setTextureRect(textureRect);
}

void WallObject::Draw(sf::RenderWindow & app) const
{
	app.draw(FShape);
}

BaseObject * WallObject::Clone() const
{
	return new WallObject();
}

void WallObject::Update(World * parWorld)
{
	std::pair<int, int> cellulCoord = parWorld->GetCelluls().GetCoordConverter().MapCoordToCellulCoord(FPosition.minX, FPosition.minY);
	int maxX = parWorld->GetCelluls().GetCoordConverter().GetX();
	int maxY = parWorld->GetCelluls().GetCoordConverter().GetY();
	int baseX = cellulCoord.first;
	int baseY = cellulCoord.second;
	// La chaleur de passe pas les mur, on set la temperature invalide
	for (int x = baseX; x < maxX ; ++x)
	{
		std::pair<int, int> realCoord = parWorld->GetCelluls().GetCoordConverter().CellulCoordToMapCoord(x, baseY);
		if (realCoord.first >= FPosition.maxX)
			break;
		for (int y = baseY; y < maxY; ++y)
		{
			realCoord = parWorld->GetCelluls().GetCoordConverter().CellulCoordToMapCoord(x, y);
			if (realCoord.second >= FPosition.maxY)
				break;
			parWorld->GetCelluls().UpdateCell(x, y, -1.0f);
		}
		
	}
}