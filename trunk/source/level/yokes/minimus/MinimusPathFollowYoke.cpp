/***
 * hesperus: MinimusPathFollowYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MinimusPathFollowYoke.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
MinimusPathFollowYoke::MinimusPathFollowYoke(const Entity_Ptr& biped)
:	m_biped(biped)
{
	if(!m_biped->camera_component() ||
	   !m_biped->collision_component() ||
	   !m_biped->nav_component() ||
	   !m_biped->physics_component())
	{
		throw Exception("Couldn't attach a biped yoke to a non-biped");
	}
}

//#################### PUBLIC METHODS ####################
std::vector<EntityCommand_Ptr> MinimusPathFollowYoke::generate_commands(UserInput& input)
{
	// NYI
	return std::vector<EntityCommand_Ptr>();
}

}
