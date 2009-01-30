/***
 * hesperus: UserBipedYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UserBipedYoke.h"

#include <source/exceptions/Exception.h>
#include <source/level/entities/BipedChangePoseCommand.h>
#include <source/level/entities/BipedTurnCommand.h>
#include <source/level/entities/BipedWalkCommand.h>
#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
UserBipedYoke::UserBipedYoke(const Entity_Ptr& biped)
:	m_biped(biped)
{
	if(!m_biped->camera_component() || !m_biped->collision_component())
	{
		throw Exception("Couldn't attach a biped yoke to a non-biped");
	}
}

//#################### PUBLIC METHODS ####################
std::vector<EntityCommand_Ptr> UserBipedYoke::generate_commands(UserInput& input)
{
	std::vector<EntityCommand_Ptr> commands;

	const Camera& camera = m_biped->camera_component()->camera();

	//~~~~~~~~~~~~~~~~
	// NORMAL MOVEMENT
	//~~~~~~~~~~~~~~~~

	Vector3d dir(0,0,0);

	// Work out the forward direction of the biped. Note that we must ensure elsewhere that the
	// look vector is never pointing directly upwards for this to work.
	Vector3d forward = camera.n();
	forward.z = 0;
	forward.normalize();

	Vector3d right = forward.cross(Vector3d(0,0,1)).normalize();

	if(input.key_down(SDLK_w)) dir += forward;
	if(input.key_down(SDLK_s)) dir -= forward;
	if(input.key_down(SDLK_d)) dir += right;
	if(input.key_down(SDLK_a)) dir -= right;

	if(dir.length() >= SMALL_EPSILON)
	{
		// Prevent faster movement when strafing
		dir.normalize();

		commands.push_back(EntityCommand_Ptr(new BipedWalkCommand(m_biped, dir)));
	}

	//~~~~~~~~~~~
	// MOUSE LOOK
	//~~~~~~~~~~~

	if(input.mouse_motion_x() || input.mouse_motion_y())
	{
		commands.push_back(EntityCommand_Ptr(new BipedTurnCommand(m_biped, input.mouse_motion_x(), input.mouse_motion_y())));
	}

	//~~~~~~~
	// CROUCH
	//~~~~~~~

	if(input.key_down(SDLK_c))
	{
		commands.push_back(EntityCommand_Ptr(new BipedChangePoseCommand(m_biped)));
		input.release_key(SDLK_c);
	}

	// TODO: Jump

	return commands;
}

}
