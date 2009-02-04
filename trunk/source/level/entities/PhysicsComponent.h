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
		// TODO:	Read the entity's mass from the file (the commented-out code is correct,
		//			but we're not ready to call it).
#if 0
		m_mass = FieldIO::read_typed_field<double>(is, "Mass");
#else
		m_mass = 75;	// there's nothing special about the 75 - I just thought it seemed a reasonable test mass in kg
#endif
	}

	//#################### PUBLIC METHODS ####################
public:
	double mass() const
	{
		return m_mass;
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
