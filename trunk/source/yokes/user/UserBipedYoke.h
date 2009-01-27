/***
 * hesperus: UserBipedYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_USERBIPEDYOKE
#define H_HESP_USERBIPEDYOKE

#include <source/level/entities/BipedEntity.h>

namespace hesp {

class UserBipedYoke : public Yoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	BipedEntity *m_biped;

	//#################### CONSTRUCTORS ####################
public:
	UserBipedYoke(BipedEntity *biped);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<EntityCommand_Ptr> generate_commands(const UserInput& input);
};

}

#endif
