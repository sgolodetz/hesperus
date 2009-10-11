/***
 * hesperus: NormalPhysicsObject.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NormalPhysicsObject.h"

#include <source/level/collisions/BoundsManager.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
NormalPhysicsObject::NormalPhysicsObject(const std::string& boundsGroup, const std::string& posture, double inverseMass,
										 PhysicsMaterial material, const Vector3d& position, const Vector3d& velocity)
:	PhysicsObject(inverseMass, material, position, velocity), m_boundsGroup(boundsGroup), m_posture(posture)
{}

//#################### PUBLIC METHODS ####################
Bounds_CPtr NormalPhysicsObject::bounds(const BoundsManager_CPtr& boundsManager) const
{
	return boundsManager->bounds(m_boundsGroup, m_posture);
}

const std::string& NormalPhysicsObject::bounds_group() const
{
	return m_boundsGroup;
}

const std::string& NormalPhysicsObject::posture() const
{
	return m_posture;
}

void NormalPhysicsObject::set_posture(const std::string& posture)
{
	m_posture = posture;
}

void NormalPhysicsObject::update(int milliseconds)
{
	// NYI
	throw 23;
}

}
