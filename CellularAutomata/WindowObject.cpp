#include "WindowObject.h"

#include "World.h"

BOOST_CLASS_EXPORT_IMPLEMENT(WindowObject)

WindowObject::WindowObject(void)
	: BaseObject("window")
	, FEnable(false)
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

	std::pair<int, int> cellulCoord = parWorld->GetCelluls().CoordConverter().MapCoordToCellulCoord(FPosition.minX, FPosition.minY);
	int maxX = parWorld->GetCelluls().CoordConverter().GetX();
	int maxY = parWorld->GetCelluls().CoordConverter().GetY();
	int baseX = cellulCoord.first;
	int baseY = cellulCoord.second;
	// on refroidit les case sur laquelle on est
	for (int x = baseX; x < maxX ; ++x)
	{
		std::pair<int, int> realCoord(INT_MAX, INT_MAX);
		for (int y = baseY; y < maxY; ++y)
		{
			realCoord = parWorld->GetCelluls().CoordConverter().CellulCoordToMapCoord(x, y);
			if (realCoord.second >= FPosition.maxY)
				break;
			float temp = parWorld->GetCelluls()[y][x];
			if (temp > 0)
				parWorld->GetCelluls().UpdateCell(x, y, temp - 1.0f);
		}
		if (realCoord.first >= FPosition.maxX)
			break;
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