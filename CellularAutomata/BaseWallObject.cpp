#include "BaseWallObject.h"

#include "World.h"

BOOST_CLASS_EXPORT_IMPLEMENT(BaseWallObject)

BaseWallObject::BaseWallObject(std::string const & parIdentifier, std::string const & parTextureName)
: BaseObject(parIdentifier)
{
	FLayer = 0;
	FTexture.loadFromFile(parTextureName);
	FTexture.setRepeated(true);
}

BaseWallObject::BaseWallObject(void)
: BaseObject("basewall")
{
	FLayer = 0;
	FTexture.loadFromFile("../Tiles/base-wall.png");
	FTexture.setRepeated(true);
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
	sf::Vector2f size(FPosition.SizeX<float>(), FPosition.SizeY<float>());
	size *= Config::CellulSize;
	FShape.setSize(size);
	FShape.setPosition(FPosition.MinX<float>() * Config::CellulSize, FPosition.MinY<float>() * Config::CellulSize);
	FShape.setTexture(&FTexture);
	sf::IntRect textureRect(sf::Vector2i(0, 0), sf::Vector2i(FPosition.SizeX<int>() * Config::iCellulSize, FPosition.SizeY<int>() * Config::iCellulSize));
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