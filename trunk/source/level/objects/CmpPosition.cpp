/***
 * hesperus: CmpPosition.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpPosition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpPosition::CmpPosition(const Vector3d& position)
:	m_position(position)
{}

//#################### PUBLIC METHODS ####################
const Vector3d& CmpPosition::position() const
{
	return m_position;
}

void CmpPosition::set_position(const Vector3d& position)
{
	m_position = position;
}

}
