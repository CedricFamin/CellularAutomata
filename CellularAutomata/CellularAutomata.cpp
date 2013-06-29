#include "CellularAutomata.h"
#include "World.h"

namespace {
	int GridSize = 10;
}

CellularAutomata::CellularAutomata()
: FAverageTemp(0.0f)
, FDeltaTemp(0.0f)
, FViscosity(1.0f)
{
	for (int i = 0; i < 11; ++i)
	{
		FTemperatureShapes[i] = new sf::RectangleShape(sf::Vector2f((float)GridSize, (float)GridSize));
		float temperature = (float)i;
		int colorR = (int)(temperature / 10.0f * 255);
		int colorB = 255 - colorR;
		int colorV = 255 - abs(colorR - colorB);
		FTemperatureShapes[i]->setFillColor(sf::Color(colorR, colorV, colorB, 100));
	}
}

void CellularAutomata::Init(int parX, int parY, float parDefaultValue)
{
	FSize = PointI(parX, parY);
	FCelluls.resize(FSize.y);
	FPreviousCelluls.resize(FSize.y);
	for (int i = 0; i < FSize.y; ++i)
	{
		FPreviousCelluls[i].resize(FSize.x);
		FCelluls[i].resize(FSize.x);
		for (int j = 0; j < FSize.x; ++j)
		{
			FCelluls[i][j].Temp = parDefaultValue;
			FPreviousCelluls[i][j].Temp = parDefaultValue;
		}
	}
}

std::vector<Cellul> const & CellularAutomata::operator[](int parY) const
{
	return FCelluls[parY];
}

std::vector<Cellul> & CellularAutomata::operator[](int parY)
{
	return FCelluls[parY];
}

void CellularAutomata::Update()
{
	static int cellsToConsider[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
	FPreviousCelluls.swap(FCelluls);
	// Regle : update des cellule
	float nbCell = 0;
	FAverageTemp = 0;
	float tempMax = FLT_MIN;
	float tempMin = FLT_MAX;
	for (int i = 0; i < FSize.y; ++i)
	{
		for (int j = 0; j < FSize.x; ++j)
		{
			float valueCell = 0;
			if (FPreviousCelluls[i][j].IsWall)
			{
				FCelluls[i][j].IsWall = true;
				continue;
			}
			float totalTemp = 0;
			float nbTempUse = 0;
			for (int move = 0; move < 8; ++move)
			{
				int x = cellsToConsider[move][0] + j;
				int y = cellsToConsider[move][1] + i;
				if (x < 0 || y < 0 || x >= FSize.x || y >= FSize.y)
					continue;
				if (FPreviousCelluls[y][x].IsWall)
					continue;
				nbTempUse++;
				totalTemp += FPreviousCelluls[i][j].Temp + (FPreviousCelluls[y][x].Temp - FPreviousCelluls[i][j].Temp) / FViscosity;
			}
			valueCell = totalTemp / nbTempUse;
			if (valueCell > 10.0f)
				valueCell = 10.0f;
			if (valueCell < .0f)
				valueCell = .0f;
			if (valueCell > tempMax) tempMax = valueCell;
			if (valueCell < tempMin) tempMin = valueCell;
			FAverageTemp += valueCell;
			++nbCell;
			FCelluls[i][j].Temp = valueCell;
		}
	}
	FAverageTemp = ceilf((FAverageTemp/ nbCell) * 100.0f) / 100.0f;
	FDeltaTemp = tempMax - tempMin;
}

void CellularAutomata::Draw(sf::RenderWindow& app) const
{
	for (int y = 0; y < FSize.y; ++y)
	{
		for (int x = 0; x < FSize.x; ++x)
		{	
			if (FCelluls[y][x].IsWall)
				continue;
			FTemperatureShapes[(int)FCelluls[y][x].Temp]->setPosition((float)x * Config::CellulSize, (float)y* Config::CellulSize);
			app.draw(*FTemperatureShapes[(int)FCelluls[y][x].Temp]);
		}
	}
}