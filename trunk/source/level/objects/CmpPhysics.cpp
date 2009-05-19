/***
 * hesperus: CmpPhysics.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpPhysics.h"

namespace hesp {

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpPhysics::create(const Properties&)
{
	return IComponent_Ptr(new CmpPhysics);
}

//#################### PUBLIC METHODS ####################
void CmpPhysics::set_velocity(const Vector3d& velocity)
{
	m_velocity = velocity;
}

const Vector3d& CmpPhysics::velocity() const
{
	return m_velocity;
}

}
