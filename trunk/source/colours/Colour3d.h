/***
 * hesperus: Colour3d.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

namespace hesp {

struct Colour3d
{
	//#################### PUBLIC VARIABLES ####################
	double r, g, b;

	//#################### CONSTRUCTORS ####################
	Colour3d() : r(0), g(0), b(0) {}
	Colour3d(double r_, double g_, double b_) : r(r_), g(g_), b(b_) {}
};

}
