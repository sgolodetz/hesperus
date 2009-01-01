/***
 * TexturedLitVector3d.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "TexturedLitVector3d.h"

#include <ostream>

namespace hesp {

//#################### PUBLIC OPERATORS ####################
TexturedLitVector3d::operator Vector3d() const
{
	return Vector3d(x,y,z);
}

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const TexturedLitVector3d& v)
{
	os << "( " << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.u << ' ' << v.v << ' ' << v.lu << ' ' << v.lv <<  " )";
	return os;
}

}
