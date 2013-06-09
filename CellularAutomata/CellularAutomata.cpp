#include "CellularAutomata.h"

CellularAutomata::CellularAutomata()
{
	for (int i = 0; i < 11; ++i)
	{
		FTemperatureShapes[i] = new sf::RectangleShape(sf::Vector2f(20, 10));
		float temperature = (float)i;
		int colorR = (int)(temperature / 10.0f * 255);
		int colorB = 255 - colorR;
		int colorV = 255 - abs(colorR - colorB);
		FTemperatureShapes[i]->setFillColor(sf::Color(colorR, colorV, colorB, 30));
	}
}

void CellularAutomata::Init(int parX, int parY, float parDefaultValue)
{
	FSizeX = parX;
	FSizeY = parY;
	FCelluls = new float*[parY + 1]();
	FPreviousCelluls = new float*[parY + 1]();
	for (int i = 0; i <= parY; ++i)
	{
		FPreviousCelluls[i] = new float[parX + 1]();
		FCelluls[i] = new float[parX + 1];
		for (int j = 0; j <= parX; ++j)
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
	float * currentPreviousCellul = NULL;
	float * currentCellul = NULL;
	// Regle : update des cellule
	for (int i = 0; i < FSizeY; ++i)
	{
		currentCellul = FCelluls[i];
		currentPreviousCellul = FPreviousCelluls[i];
		for (int j = 0; j < FSizeX; ++j, ++currentCellul, ++currentPreviousCellul)
		{
			if (*currentPreviousCellul < 0)
				continue;
			float totalTemp = 0;
			float nbTempUse = 0;
			for (int move = 0; move < 8; ++move)
			{
				int x = cellsToConsider[move][0] + j;
				int y = cellsToConsider[move][1] + i;
				if (x < 0 || y < 0 || x >= FSizeX || y >= FSizeY)
					continue;
				float * cell = *(FPreviousCelluls + y) + x;
				if (*cell < 0)
					continue;
				nbTempUse++;
				totalTemp += *cell;
			}
			*currentCellul = totalTemp / nbTempUse;
		}
	}
}

void CellularAutomata::Draw(sf::RenderWindow& app) const
{
	for (int y = 0; y < FSizeY; y += 10)
	{
		for (int x = 0; x < FSizeX; x += 10)
		{	
			if (FCelluls[y][x] < 0)
				continue;
			FTemperatureShapes[(int)FCelluls[y][x]]->setPosition((float)x, (float)y);
			app.draw(*FTemperatureShapes[(int)FCelluls[y][x]]);
		}
	}
}