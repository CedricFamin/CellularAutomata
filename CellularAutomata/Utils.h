#pragma once

#include <boost/serialization/access.hpp>

template<typename type = int>
struct Point
{ 
	Point() : x(0), y(0) {}
	Point(type const & parX, type const & parY) : x(parX) , y(parY) { }

	type x; 
	type y; 

	// -----------------------------------------------------------
	// SaveLoad
	// -----------------------------------------------------------
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		ar & x;
		ar & y;
    }
};

typedef Point<int> PointI;
