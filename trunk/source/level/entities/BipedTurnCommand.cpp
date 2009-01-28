/***
 * hesperus: BipedTurnCommand.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BipedTurnCommand.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BipedTurnCommand::BipedTurnCommand(const Entity_Ptr& biped, int mouseMotionX, int mouseMotionY)
:	m_biped(biped), m_mouseMotionX(mouseMotionX), m_mouseMotionY(mouseMotionY)
{}

//#################### PUBLIC METHODS ####################
void BipedTurnCommand::execute(int milliseconds)
{
	Camera& camera = m_biped->camera_component()->camera();

	const double CAMERA_SPEED = 15.0;

	// For left-right motion, rotate about the (0,0,1) axis.
#if 1
	camera.rotate(Vector3d(0,0,1), -m_mouseMotionX * CAMERA_SPEED * (milliseconds/1000.0));
#endif
}

}
