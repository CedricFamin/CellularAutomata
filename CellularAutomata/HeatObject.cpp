#include "HeatObject.h"

#include "World.h"

HeatObject::HeatObject(void)
	: BaseObject("heat")
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
	// on chauffe  les case sur laquel on est
	for (int x = FPosition.minX; x < FPosition.maxX; ++x)
	{
		for (int y = FPosition.minY; y < FPosition.maxY; ++y)
		{
			if (parWorld->GetCelluls()[y][x] < 10 && parWorld->GetCelluls()[y][x] > 0)
			{
				parWorld->GetCelluls().UpdateCell(x, y, parWorld->GetCelluls()[y][x] + 1);
			}
			

		}
	}
}

void HeatObject::OnClick(World * parWorld, int parX, int parY)
{
	if (FPosition.minX <= parX && FPosition.maxX >= parX)
	{
		if (FPosition.minY <= parY && FPosition.maxY >= parY)
		{
			FEnable = !FEnable;
		}
	}
}