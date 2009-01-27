/***
 * hesperus: UserBipedYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UserBipedYoke.h"

#include <source/level/entities/BipedWalkCommand.h>
#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
UserBipedYoke::UserBipedYoke(BipedEntity *biped)
:	m_biped(biped)
{}

//#################### PUBLIC METHODS ####################
std::vector<EntityCommand_Ptr> UserBipedYoke::generate_commands(const UserInput& input)
{
	std::vector<EntityCommand_Ptr> commands;

	Vector3d dir(0,0,0);

	// Work out the forward direction of the biped. Note that we must ensure elsewhere that the
	// look vector is never pointing directly upwards for this to work.
	Vector3d forward = m_biped->look();
	forward.z = 0;
	forward.normalize();

	Vector3d right = forward.cross(Vector3d(0,0,1)).normalize();

	if(input.key_down(SDLK_w)) dir += forward;
	if(input.key_down(SDLK_s)) dir -= forward;
	if(input.key_down(SDLK_d)) dir += right;
	if(input.key_down(SDLK_a)) dir -= right;

	if(dir.length() >= SMALL_EPSILON)
	{
		dir.normalize();
		commands.push_back(EntityCommand_Ptr(new BipedWalkCommand(m_biped, dir)));
	}

	// TODO: Mouse look, jump, crouch

	return commands;
}

}
