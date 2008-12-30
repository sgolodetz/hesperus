/***
 * hesperus: Colour3d.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Colour3d.h"

namespace hesp {

//#################### PUBLIC OPERATORS ####################
Colour3d& Colour3d::operator+=(const Colour3d& rhs)
{
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	return *this;
}

Colour3d& Colour3d::operator*=(double factor)
{
	r *= factor;
	g *= factor;
	b *= factor;
	return *this;
}

Colour3d& Colour3d::operator/=(double factor)
{
	*this *= 1/factor;
	return *this;
}

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
