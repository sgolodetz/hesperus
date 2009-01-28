/***
 * hesperus: UserBipedYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UserBipedYoke.h"

#include <source/exceptions/Exception.h>
#include <source/level/entities/BipedWalkCommand.h>
#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
UserBipedYoke::UserBipedYoke(const Entity_Ptr& biped)
:	m_biped(biped)
{
	if(!m_biped->collision_component() ||
	   !m_biped->look_component() ||
	   !m_biped->position_component())
	{
		throw Exception("Couldn't attach a biped yoke to a non-biped");
	}
}

//#################### PUBLIC METHODS ####################
std::vector<EntityCommand_Ptr> UserBipedYoke::generate_commands(const UserInput& input)
{
	std::vector<EntityCommand_Ptr> commands;

	Vector3d dir(0,0,0);

	// Work out the forward direction of the biped. Note that we must ensure elsewhere that the
	// look vector is never pointing directly upwards for this to work.
	Vector3d forward = m_biped->look_component()->look();
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
