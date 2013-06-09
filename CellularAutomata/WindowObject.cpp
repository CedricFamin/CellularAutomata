#include "WindowObject.h"

#include "World.h"

WindowObject::WindowObject(void)
	: BaseObject("window")
{
	FLayer = 1;
}


WindowObject::~WindowObject(void)
{
}

void WindowObject::Draw(sf::RenderWindow & app) const
{
	sf::Vector2f size(FPosition.maxX - FPosition.minX, FPosition.maxY - FPosition.minY);
	sf::RectangleShape shape(size);

	shape.setPosition(FPosition.minX, FPosition.minY);
	shape.setFillColor(sf::Color(0, 0, 255));
	
	app.draw(shape);
}

BaseObject * WindowObject::Clone() const
{
	return new WindowObject();
}


void WindowObject::Update(World * parWorld)
{
	if (!FEnable)
		return ;
	// on refroidit  les case sur laquel on est
	for (int x = FPosition.minX; x < FPosition.maxX; ++x)
	{
		for (int y = FPosition.minY; y < FPosition.maxY; ++y)
		{
			if (parWorld->GetCelluls()[y][x] > 0)
			{
				parWorld->GetCelluls().UpdateCell(x, y, std::max(parWorld->GetCelluls()[y][x] - 1, 0.0f));
			}
		}
	}
}

void WindowObject::OnClick(World * parWorld, int parX, int parY)
{
	if (FPosition.minX <= parX && FPosition.maxX >= parX)
	{
		if (FPosition.minY <= parY && FPosition.maxY >= parY)
		{
			FEnable = !FEnable;
		}
	}
}