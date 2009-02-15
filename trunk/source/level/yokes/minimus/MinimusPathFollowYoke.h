/***
 * hesperus: MinimusPathFollowYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MINIMUSPATHFOLLOWYOKE
#define H_HESP_MINIMUSPATHFOLLOWYOKE

#include <source/level/entities/Entity.h>
#include <source/level/entities/Yoke.h>

namespace hesp {

/**
This class represents a path-following yoke for the Minimus bot.
*/
class MinimusPathFollowYoke : public Yoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	Entity_Ptr m_biped;

	//#################### CONSTRUCTORS ####################
public:
	MinimusPathFollowYoke(const Entity_Ptr& biped);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<EntityCommand_Ptr> generate_commands(UserInput& input);
};

}

#endif
