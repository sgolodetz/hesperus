/***
 * hesperus: PolygonTypes.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "PolygonTypes.h"

#include <iostream>

namespace hesp {

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const CPAuxData& rhs)
{
	os << rhs.walkable;
	return os;
}

std::istream& operator>>(std::istream& is, CPAuxData& rhs)
{
	is >> rhs.walkable;
	return is;
}

}
