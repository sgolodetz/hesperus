/***
 * hesperus: UserBipedYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_USERBIPEDYOKE
#define H_HESP_USERBIPEDYOKE

#include <source/level/entities/EntityEx.h>
#include <source/level/entities/Yoke.h>

namespace hesp {

class UserBipedYoke : public Yoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	Entity_Ptr m_biped;

	//#################### CONSTRUCTORS ####################
public:
	UserBipedYoke(const Entity_Ptr& biped);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<EntityCommand_Ptr> generate_commands(const UserInput& input);
};

}

#endif
