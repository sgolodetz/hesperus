/***
 * hesperus: CmpOrientation.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpOrientation.h"

#include "VerticallyConstrainedNUVAxes.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpOrientation::CmpOrientation(const Vector3d& look)
{
	m_nuvAxes.reset(new VerticallyConstrainedNUVAxes(look));
}

//#################### PUBLIC METHODS ####################
NUVAxes_Ptr CmpOrientation::nuv_axes() const
{
	return m_nuvAxes;
}

}
