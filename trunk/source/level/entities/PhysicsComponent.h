/***
 * hesperus: PhysicsComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PHYSICSCOMPONENT
#define H_HESP_PHYSICSCOMPONENT

#include <source/io/FieldIO.h>
#include "IPhysicsComponent.h"

namespace hesp {

class PhysicsComponent : public IPhysicsComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	double m_mass;			// the mass of the entity in kg
	Vector3d m_velocity;	// the velocity of the entity in m/s

	//#################### CONSTRUCTORS ####################
public:
	PhysicsComponent(std::istream& is)
	{
		m_mass = FieldIO::read_typed_field<double>(is, "Mass");
	}

	//#################### PUBLIC METHODS ####################
public:
	double mass() const
	{
		return m_mass;
	}

	void save(std::ostream& os) const
	{
		FieldIO::write_typed_field(os, "Mass", m_mass);
	}

	void set_velocity(const Vector3d& velocity)
	{
		m_velocity = velocity;
	}

	const Vector3d& velocity() const
	{
		return m_velocity;
	}
};

}

#endif
