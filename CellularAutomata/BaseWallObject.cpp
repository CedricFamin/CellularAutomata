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

void BaseWallObject::Update(World * parWorld)
{
	int maxX = parWorld->GetCelluls().GetSize().x;
	int maxY = parWorld->GetCelluls().GetSize().y;
	int baseX = FPosition.MinX<int>();
	int baseY = FPosition.MinY<int>();
	// La chaleur de passe pas les mur, on set la temperature invalide
	for (int x = baseX; x < maxX ; ++x)
	{
		if (x>= FPosition.MaxX<int>())
			break;
		for (int y = baseY; y < maxY; ++y)
		{
			if (y >= FPosition.MaxY<int>())
				break;
			parWorld->GetCelluls()[y][x].IsWall = true;
			parWorld->GetCelluls()[y][x].Conductivity = 1.1f; /// TODO PARAMETER
		}
	}

	return ;
}