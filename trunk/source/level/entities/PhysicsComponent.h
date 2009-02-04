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
	double m_mass;	// the mass of the entity in kg

	//#################### CONSTRUCTORS ####################
public:
	PhysicsComponent(std::istream& is)
	{
		m_mass = FieldIO::read_typed_field<double>(is, "Mass");
	}

	//#################### PUBLIC METHODS ####################
public:
	double mass() const;
};

}

#endif
