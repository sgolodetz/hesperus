/***
 * hesperus: BipedWalkCommand.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BipedWalkCommand.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BipedWalkCommand::BipedWalkCommand(BipedEntity *biped, const Vector3d& dir)
:	m_biped(biped), m_dir(dir)
{}

//#################### PUBLIC METHODS ####################
void BipedWalkCommand::execute(int milliseconds)
{
	m_biped->walk(m_dir, milliseconds);
}

}
