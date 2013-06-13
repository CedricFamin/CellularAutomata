#include "HeatObject.h"

#include "World.h"

BOOST_CLASS_EXPORT_IMPLEMENT(HeatObject)

HeatObject::HeatObject(void)
	: BaseObject("heat")
	, FEnable(false)
{
	FLayer = 1;
}


HeatObject::~HeatObject(void)
{
}

void HeatObject::Draw(sf::RenderWindow & app) const
{
	sf::Texture texture;
	texture.loadFromFile(FEnable ? "../Tiles/fireplace-on.png" : "../Tiles/fireplace-off.png");
	texture.setRepeated(true);
	sf::Sprite sprite;
	sprite.setTexture(texture, true);
	sprite.setPosition(FPosition.minX, FPosition.minY);
	
	app.draw(sprite);
}

BaseObject * HeatObject::Clone() const
{
	return new HeatObject();
}


void HeatObject::Update(World * parWorld)
{
	if (!FEnable)
		return ;

	std::pair<int, int> cellulCoord = parWorld->GetCelluls().CoordConverter().MapCoordToCellulCoord(FPosition.minX, FPosition.minY);
	int maxX = parWorld->GetCelluls().CoordConverter().GetX();
	int maxY = parWorld->GetCelluls().CoordConverter().GetY();
	int baseX = cellulCoord.first;
	int baseY = cellulCoord.second;
	// on chauffe les case sur laquelle on est
	for (int x = baseX; x < maxX ; ++x)
	{
		std::pair<int, int> realCoord(INT_MAX, INT_MAX);
		for (int y = baseY; y < maxY; ++y)
		{
			realCoord = parWorld->GetCelluls().CoordConverter().CellulCoordToMapCoord(x, y);
			if (realCoord.second >= FPosition.maxY)
				break;
			float temp = parWorld->GetCelluls()[y][x];
			if (temp >= 0)
				parWorld->GetCelluls().UpdateCell(x, y, temp + 1.0f);
		}
		if (realCoord.first >= FPosition.maxX)
			break;
	}
}

bool HeatObject::OnClick(World * parWorld, int parX, int parY)
{
	if (FPosition.minX <= parX && FPosition.maxX >= parX)
	{
		if (FPosition.minY <= parY && FPosition.maxY >= parY)
		{
			FEnable = !FEnable;
			return true;
		}
	}
	return false;
}