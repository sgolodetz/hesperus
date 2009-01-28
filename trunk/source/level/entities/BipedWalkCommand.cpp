/***
 * hesperus: BipedWalkCommand.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BipedWalkCommand.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BipedWalkCommand::BipedWalkCommand(const Entity_Ptr& biped, const Vector3d& dir)
:	m_biped(biped), m_dir(dir)
{}

//#################### PUBLIC METHODS ####################
void BipedWalkCommand::execute(int milliseconds)
{
	// FIXME: Walking speed will eventually be a property of the entity.
	// FIXME: This will eventually call an external function to move the object properly (i.e. taking the nav mesh and collisions into account).
	const double WALK_SPEED = 5.0;
	IPositionComponent_Ptr posComponent = m_biped->position_component();
	posComponent->set_position(posComponent->position() + m_dir * WALK_SPEED * (milliseconds / 1000.0));
}

}
