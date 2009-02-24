/***
 * hesperus: PhysicsComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PhysicsComponent.h"

#include <source/io/FieldIO.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
PhysicsComponent::PhysicsComponent(std::istream& is)
{
	m_mass = FieldIO::read_typed_field<double>(is, "Mass");
}

//#################### PUBLIC METHODS ####################
double PhysicsComponent::mass() const
{
	return m_mass;
}

double PhysicsComponent::run_speed() const
{
	// FIXME: This should be loaded in the constructor.
	return 10.0;	// in units/s
}

void PhysicsComponent::save(std::ostream& os) const
{
	FieldIO::write_typed_field(os, "Mass", m_mass);
}

void PhysicsComponent::set_velocity(const Vector3d& velocity)
{
	m_velocity = velocity;
}

const Vector3d& PhysicsComponent::velocity() const
{
	return m_velocity;
}

double PhysicsComponent::walk_speed() const
{
	// FIXME: This should be loaded in the constructor.
	return 5.0;		// in units/s
}

}
