#include "CellularAutomata.h"

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
	FCoordConverter.Init(parX, parY);
	FCelluls = new float*[FCoordConverter.GetY() + 1]();
	FPreviousCelluls = new float*[FCoordConverter.GetY() + 1]();
	for (int i = 0; i <= FCoordConverter.GetY(); ++i)
	{
		FPreviousCelluls[i] = new float[FCoordConverter.GetX() + 1]();
		FCelluls[i] = new float[FCoordConverter.GetX() + 1];
		for (int j = 0; j <= FCoordConverter.GetX(); ++j)
		{
			FCelluls[i][j] = parDefaultValue;
			FPreviousCelluls[i][j] = parDefaultValue;
		}
	}
}

float const * CellularAutomata::operator[](int parY) const
{
	return FCelluls[parY];
}

void CellularAutomata::UpdateCell(int parX, int parY, float parValue)
{
	FCelluls[parY][parX] = parValue;
}

void CellularAutomata::Update()
{
	static int cellsToConsider[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
	float** tmp = FPreviousCelluls;
	FPreviousCelluls = FCelluls;
	FCelluls = tmp;
	// Regle : update des cellule
	float nbCell = 0;
	FAverageTemp = 0;
	float tempMax = FLT_MIN;
	float tempMin = FLT_MAX;
	for (int i = 0; i < FCoordConverter.GetY(); ++i)
	{
		for (int j = 0; j < FCoordConverter.GetX(); ++j)
		{
			float valuePreviousCell = FPreviousCelluls[i][j];
			float valueCell = 0;
			if (valuePreviousCell < 0)
			{
				if (valuePreviousCell > -.8f)
					valuePreviousCell = 0;
				else
					continue;
			}
			float totalTemp = 0;
			float nbTempUse = 0;
			for (int move = 0; move < 8; ++move)
			{
				int x = cellsToConsider[move][0] + j;
				int y = cellsToConsider[move][1] + i;
				if (x < 0 || y < 0 || x >= FCoordConverter.GetX() || y >= FCoordConverter.GetY())
					continue;
				if (FPreviousCelluls[y][x] < 0)
					continue;
				nbTempUse++;
				// viscosite
				/*
				if (std::fabs(FPreviousCelluls[y][x] - FCelluls[i][j]) > 0.05f)
					
				else
					totalTemp += FCelluls[i][j];
				*/
				totalTemp += FCelluls[i][j] + (FPreviousCelluls[y][x] - FCelluls[i][j]) / FViscosity;
			}
			valueCell = totalTemp / nbTempUse;
			if (valueCell > 10.0f)
				valueCell = 10.0f;
			if (valueCell > tempMax) tempMax = valueCell;
			if (valueCell < tempMin) tempMin = valueCell;
			FAverageTemp += valueCell;
			++nbCell;
			FCelluls[i][j] = valueCell;
		}
	}
	FAverageTemp = ceilf((FAverageTemp/ nbCell) * 100.0f) / 100.0f;
	FDeltaTemp = tempMax - tempMin;
}

void CellularAutomata::Draw(sf::RenderWindow& app) const
{
	for (int y = 0; y < FCoordConverter.GetY(); ++y)
	{
		for (int x = 0; x < FCoordConverter.GetX(); ++x)
		{	
			if (FCelluls[y][x] < 0)
				continue;
			std::pair<int, int> realCoord = FCoordConverter.CellulCoordToMapCoord(x, y);
			FTemperatureShapes[(int)FCelluls[y][x]]->setPosition((float)realCoord.first, (float)realCoord.second);
			app.draw(*FTemperatureShapes[(int)FCelluls[y][x]]);
		}
	}
}