#include "BaseWallObject.h"

#include "World.h"

BOOST_CLASS_EXPORT_IMPLEMENT(BaseWallObject)

BaseWallObject::BaseWallObject(std::string const & parIdentifier, std::string const & parTextureName)
: BaseObject(parIdentifier)
, FWallTemp(3.0f)
{
	FBaseWallTemp = FWallTemp;
	FLayer = 0;
	FTexture.loadFromFile(parTextureName);
	FTexture.setRepeated(true);
	FTempToGet = 0.1f;
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
			parWorld->GetCelluls()[y][x].IsWall = true;
		}
	}

	// on perd une partie de la jauge de temperature
	if (FWallTemp > FBaseWallTemp)
		FWallTemp /= 10.0f;

	// je fais con mais simple, je ne change pas les regle de la grille
	// on preleve un peu de temperature sur l'exterieur
	struct Deplacement { int DecalX; int DecalY; int BorneX; int BorneY; };
	Deplacement deplacements[] = {
		{ 1, 0, FPosition.MaxX<int>(), INT_MAX}, 
		{ 0, 1, INT_MAX, FPosition.MaxY<int>()}, 
		{-1, 0, FPosition.MinX<int>() - 1, INT_MAX}, 
		{ 0,-1, INT_MAX, FPosition.MinY<int>() - 1}
	};
	std::pair<int, int> position(FPosition.MinX<int>() - 1, FPosition.MinY<int>() - 1);
	for (unsigned int i = 0; i < sizeof(deplacements) / sizeof(*deplacements); ++i)
	{
		Deplacement const & deplacement = deplacements[i];
		while (position.first != deplacement.BorneX && position.second != deplacement.BorneY)
		{
			if (position.first >= 0 && position.first < parWorld->GetX())
			{
				if (position.second >= 0 && position.second < parWorld->GetY())
				{
					float tempCell = parWorld->GetCelluls()[position.second][position.first].Temp;
					if (tempCell > FWallTemp)
					{
						parWorld->GetCelluls()[position.second][position.first].Temp = tempCell - FTempToGet;
						// on ajoute un peu de temperature a la jaunge
						FWallTemp += FTempToGet;
					}
					else 
					{
						int i = 0;
					}
				}
			}
			position.first += deplacement.DecalX;
			position.second += deplacement.DecalY;
		}
	}
}