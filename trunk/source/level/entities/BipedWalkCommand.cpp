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
	IPositionComponent_Ptr posComponent = m_biped->position_component();
	posComponent->set_position(posComponent->position() + m_dir * 5.0 * (milliseconds / 1000.0));
}

}
