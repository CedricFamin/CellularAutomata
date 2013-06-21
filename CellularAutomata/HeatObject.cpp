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
	sprite.setPosition(FPosition.MinX<float>(), FPosition.MinY<float>());
	
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

	std::pair<int, int> cellulCoord = parWorld->GetCelluls().GetCoordConverter().MapCoordToCellulCoord(FPosition.MinX<int>(), FPosition.MinY<int>());
	int maxX = parWorld->GetCelluls().GetCoordConverter().GetX();
	int maxY = parWorld->GetCelluls().GetCoordConverter().GetY();
	int baseX = cellulCoord.first;
	int baseY = cellulCoord.second;
	// on chauffe les case sur laquelle on est
	for (int x = baseX; x < maxX ; ++x)
	{
		std::pair<int, int> realCoord(INT_MAX, INT_MAX);
		for (int y = baseY; y < maxY; ++y)
		{
			realCoord = parWorld->GetCelluls().GetCoordConverter().CellulCoordToMapCoord(x, y);
			if (realCoord.second >= FPosition.MaxY<int>())
				break;
			float temp = parWorld->GetCelluls()[y][x];
			if (temp >= 0)
				parWorld->GetCelluls().UpdateCell(x, y, temp + 1.0f);
		}
		if (realCoord.first >= FPosition.MaxX<int>())
			break;
	}
}

bool HeatObject::OnClick(World * parWorld, int parX, int parY)
{
	if (FPosition.MinX<int>() <= parX && FPosition.MaxX<int>() >= parX)
	{
		if (FPosition.MinY<int>() <= parY && FPosition.MaxY<int>() >= parY)
		{
			FEnable = !FEnable;
			return true;
		}
	}
	return false;
}