/***
 * hesperus: Colour3d.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Colour3d.h"

namespace hesp {

//#################### GLOBAL OPERATORS ####################
Colour3d operator*(double factor, const Colour3d& c)
{
	return Colour3d(factor*c.r, factor*c.g, factor*c.b);
}

Colour3d operator*(const Colour3d& c, double factor)
{
	return Colour3d(factor*c.r, factor*c.g, factor*c.b);
}

}
