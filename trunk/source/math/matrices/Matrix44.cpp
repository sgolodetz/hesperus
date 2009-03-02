/***
 * hesperus: Matrix44.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Matrix44.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Matrix44::Matrix44() {}

//#################### STATIC FACTORY METHODS ####################
Matrix44_Ptr Matrix44::from_axis_angle(const Vector3d& axis, double angle)
{
	// NYI
	throw 23;
}

Matrix44_Ptr Matrix44::identity()
{
	// NYI
	throw 23;
}

Matrix44_Ptr Matrix44::zeros()
{
	// NYI
	throw 23;
}

}
