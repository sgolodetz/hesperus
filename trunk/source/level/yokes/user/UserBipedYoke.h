/***
 * hesperus: UserBipedYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_USERBIPEDYOKE
#define H_HESP_USERBIPEDYOKE

#include <source/level/entities/Entity.h>
#include <source/level/entities/Yoke.h>

namespace hesp {

/**
This class represents a yoke that allows the user to control a biped.
Bipeds can walk, run, jump, crouch, etc.
*/
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
	std::vector<EntityCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
};

}

#endif
