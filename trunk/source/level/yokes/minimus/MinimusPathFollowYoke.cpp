/***
 * hesperus: MinimusPathFollowYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MinimusPathFollowYoke.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
MinimusPathFollowYoke::MinimusPathFollowYoke(const Entity_Ptr& biped)
:	m_biped(biped)
{}

//#################### PUBLIC METHODS ####################
std::vector<EntityCommand_Ptr> MinimusPathFollowYoke::generate_commands(UserInput& input)
{
	// NYI
	return std::vector<EntityCommand_Ptr>();
}

}
