/***
 * hesperus: NormalPhysicsObject.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NormalPhysicsObject.h"

#include <source/level/bounds/BoundsManager.h>

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
	double t = milliseconds / 1000.0;
	Vector3d acceleration = inverse_mass() * accumulated_force();
	cache_previous_position();
	set_position(position() + velocity() * t + 0.5 * acceleration * t * t);
	set_velocity(velocity() + acceleration * t);
}

}
