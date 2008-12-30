/***
 * TexturedLitVector3d.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "TexturedLitVector3d.h"

namespace hesp {

//#################### PUBLIC OPERATORS ####################
TexturedLitVector3d::operator Vector3d() const
{
	return Vector3d(x,y,z);
}

}
