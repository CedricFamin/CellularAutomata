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
	sprite.setPosition(FPosition.MinX<float>() * Config::CellulSize, FPosition.MinY<float>() * Config::CellulSize);
	
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

	
	int maxX = parWorld->GetCelluls().GetSize().x;
	int maxY = parWorld->GetCelluls().GetSize().y;
	int baseX = FPosition.MinX<int>();
	int baseY = FPosition.MinY<int>();
	// on chauffe les case sur laquelle on est
	for (int x = baseX; x < maxX ; ++x)
	{
		std::pair<int, int> realCoord(INT_MAX, INT_MAX);
		for (int y = baseY; y < maxY; ++y)
		{
			if (y >= FPosition.MaxY<int>())
				break;
			parWorld->GetCelluls()[y][x].Temp = 10;
		}
		if (x >= FPosition.MaxX<int>())
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