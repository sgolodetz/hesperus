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

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpOrientation::create(const Properties& properties)
{
	return IComponent_Ptr(new CmpOrientation(properties.get_actual<Vector3d>("Look")));
}

//#################### PUBLIC METHODS ####################
NUVAxes_Ptr CmpOrientation::nuv_axes() const
{
	return m_nuvAxes;
}

}
