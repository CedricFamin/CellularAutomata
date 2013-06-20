#pragma once

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

class CoordConverter
{
public:
	void Init(int parRealX, int parRealY);

	std::pair<int, int> MapCoordToCellulCoord(int parRealX, int parRealY) const;
	std::pair<int, int> CellulCoordToMapCoord(int parX, int parY) const;

	int GetRealX() const { return FRealSizeX; }
	int GetRealY() const { return FRealSizeY; }
	int GetX() const { return FSizeX; }
	int GetY() const { return FSizeY; }

	// -----------------------------------------------------------
	// SaveLoad
	// -----------------------------------------------------------
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & FRealSizeX;
		ar & FRealSizeY;
		ar & FSizeX;
		ar & FSizeY;
    }
protected:
private:
	int FRealSizeX;
	int FRealSizeY;
	int FSizeX;
	int FSizeY;
};
