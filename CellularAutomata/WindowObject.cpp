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
	sf::Vector2f size(FPosition.SizeX<float>() * Config::CellulSize, FPosition.SizeY<float>() * Config::CellulSize);
	sf::RectangleShape shape(size);

	shape.setPosition(FPosition.MinX<float>() * Config::CellulSize, FPosition.MinY<float>() * Config::CellulSize);
	sf::Texture texture;
	texture.loadFromFile(FEnable ? "../Tiles/window-on.png" : "../Tiles/window-off.png");
	shape.setTexture(&texture);
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

	int maxX = parWorld->GetCelluls().GetSize().x;
	int maxY = parWorld->GetCelluls().GetSize().y;
	int baseX = FPosition.MinX<int>();
	int baseY = FPosition.MinY<int>();
	// on refroidit les case sur laquelle on est
	for (int x = baseX; x < maxX ; ++x)
	{
		std::pair<int, int> realCoord(INT_MAX, INT_MAX);
		for (int y = baseY; y < maxY; ++y)
		{
			if (y >= FPosition.MaxY<int>() + 1)
				break;
			parWorld->GetCelluls()[y][x].Temp = 0.0f;
		}
		if (x >= FPosition.MaxX<int>())
			break;
	}
}

bool WindowObject::OnClick(World * parWorld, int parX, int parY)
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