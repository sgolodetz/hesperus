/***
 * hesperus: PolygonTypes.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "PolygonTypes.h"

#include <iostream>

namespace hesp {

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const ColPolyAuxData& rhs)
{
	os << rhs.m_walkable;
	return os;
}

std::istream& operator>>(std::istream& is, ColPolyAuxData& rhs)
{
	is >> rhs.m_walkable;
	return is;
}

}
