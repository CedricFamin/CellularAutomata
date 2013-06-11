#include "CellularAutomata.h"

void CellularCoordConverter::Init(int parRealX, int parRealY)
{
	FRealSizeX = parRealX;
	FRealSizeY = parRealY;

	FSizeY = FRealSizeY / 10;
	FSizeX = FRealSizeX / 10;
}

std::pair<int, int> CellularCoordConverter::MapCoordToCellulCoord(int parRealX, int parRealY) const
{
	return std::make_pair(parRealX / 10, parRealY / 10);
}

std::pair<int, int> CellularCoordConverter::CellulCoordToMapCoord(int parX, int parY) const
{
	return std::make_pair(parX * 10, parY * 10);
}

CellularAutomata::CellularAutomata()
{
	for (int i = 0; i < 11; ++i)
	{
		FTemperatureShapes[i] = new sf::RectangleShape(sf::Vector2f(10, 10));
		float temperature = (float)i;
		int colorR = (int)(temperature / 10.0f * 255);
		int colorB = 255 - colorR;
		int colorV = 255 - abs(colorR - colorB);
		FTemperatureShapes[i]->setFillColor(sf::Color(colorR, colorV, colorB, 30));
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
	float * currentPreviousCellul = NULL;
	float * currentCellul = NULL;
	// Regle : update des cellule
	for (int i = 0; i < FCoordConverter.GetY(); ++i)
	{
		currentCellul = FCelluls[i];
		currentPreviousCellul = FPreviousCelluls[i];
		for (int j = 0; j < FCoordConverter.GetX(); ++j, ++currentCellul, ++currentPreviousCellul)
		{
			if (*currentPreviousCellul < 0)
			{
				if (*currentPreviousCellul > -.8f)
					*currentPreviousCellul = 0;
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
				float * cell = *(FPreviousCelluls + y) + x;
				if (*cell < 0)
					continue;
				nbTempUse++;
				totalTemp += *cell;
			}
			*currentCellul = totalTemp / nbTempUse;
			if (*currentCellul > 10.0f)
				*currentCellul = 10.0f;
		}
	}
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