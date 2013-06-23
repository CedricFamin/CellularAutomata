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
	sf::Vector2f size(FPosition.SizeX<float>(), FPosition.SizeY<float>());
	size *= Config::CellulSize;
	FShape.setSize(size);
	FShape.setPosition(FPosition.MinX<float>() * Config::CellulSize, FPosition.MinY<float>() * Config::CellulSize);
	FTexture.loadFromFile("../Tiles/wall0.png");
	FTexture.setRepeated(true);
	FShape.setTexture(&FTexture);
	sf::IntRect textureRect(sf::Vector2i(0, 0), sf::Vector2i(FPosition.SizeX<int>() * Config::iCellulSize, FPosition.SizeY<int>() * Config::iCellulSize));
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
	std::pair<int, int> cellulCoord = parWorld->GetCelluls().GetCoordConverter().MapCoordToCellulCoord(FPosition.MinX<int>(), FPosition.MinY<int>());
	int maxX = parWorld->GetCelluls().GetCoordConverter().GetX();
	int maxY = parWorld->GetCelluls().GetCoordConverter().GetY();
	int baseX = cellulCoord.first;
	int baseY = cellulCoord.second;
	// La chaleur de passe pas les mur, on set la temperature invalide
	for (int x = baseX; x < maxX ; ++x)
	{
		std::pair<int, int> realCoord = parWorld->GetCelluls().GetCoordConverter().CellulCoordToMapCoord(x, baseY);
		if (realCoord.first >= FPosition.MaxX<int>())
			break;
		for (int y = baseY; y < maxY; ++y)
		{
			realCoord = parWorld->GetCelluls().GetCoordConverter().CellulCoordToMapCoord(x, y);
			if (realCoord.second >= FPosition.MaxY<int>())
				break;
			parWorld->GetCelluls().UpdateCell(x, y, -1.0f);
		}
		
	}
}