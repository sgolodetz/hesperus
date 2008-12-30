/***
 * hesperus: Colour3d.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_COLOURS_COLOUR3D
#define H_HESP_COLOURS_COLOUR3D

namespace hesp {

struct Colour3d
{
	//#################### PUBLIC VARIABLES ####################
	double r, g, b;

	//#################### CONSTRUCTORS ####################
	Colour3d() : r(0), g(0), b(0) {}
	Colour3d(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}

	//#################### PUBLIC OPERATORS ####################
	Colour3d& operator+=(const Colour3d& rhs);
	Colour3d& operator*=(double factor);
	Colour3d& operator/=(double factor);
};

//#################### GLOBAL OPERATORS ####################
Colour3d operator*(double factor, const Colour3d& c);
Colour3d operator*(const Colour3d& c, double factor);

}

#endif
