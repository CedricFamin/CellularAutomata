#include "BaseWallObject.h"

#include "World.h"

BOOST_CLASS_EXPORT_IMPLEMENT(BaseWallObject)

BaseWallObject::BaseWallObject(void)
: BaseObject("basewall")
{
	FLayer = 0;
}


BaseWallObject::~BaseWallObject(void)
{
}

void BaseWallObject::Init(ParamList const & parParams)
{
	BaseObject::Init(parParams);
	InitShape();
}

void BaseWallObject::InitShape()
{
	sf::Vector2f size((float)(FPosition.maxX - FPosition.minX), (float)(FPosition.maxY - FPosition.minY));
	FShape.setSize(size);
	FShape.setPosition((float)FPosition.minX, (float)FPosition.minY);
	FTexture.loadFromFile("../Tiles/base-wall.png");
	FTexture.setRepeated(true);
	FShape.setTexture(&FTexture);
	sf::IntRect textureRect(sf::Vector2i(0, 0), sf::Vector2i(FTexture.getSize().x, FPosition.maxY - FPosition.minY));
	FShape.setTextureRect(textureRect);
}

void BaseWallObject::Draw(sf::RenderWindow & app) const
{
	app.draw(FShape);
}

BaseObject * BaseWallObject::Clone() const
{
	return new BaseWallObject();
}

void BaseWallObject::Update(World * parWorld)
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