#include "CoordConverter.h"

namespace {
	int GridSize = 10;
}

void CoordConverter::Init(int parRealX, int parRealY)
{
	FRealSizeX = parRealX;
	FRealSizeY = parRealY;

	FSizeY = FRealSizeY / GridSize;
	FSizeX = FRealSizeX / GridSize;
}

std::pair<int, int> CoordConverter::MapCoordToCellulCoord(int parRealX, int parRealY) const
{
	return std::make_pair(parRealX / GridSize, parRealY / GridSize);
}

std::pair<int, int> CoordConverter::CellulCoordToMapCoord(int parX, int parY) const
{
	return std::make_pair(parX * GridSize, parY * GridSize);
}
