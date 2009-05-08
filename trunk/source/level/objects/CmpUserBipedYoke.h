/***
 * hesperus: CmpUserBipedYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPUSERBIPEDYOKE
#define H_HESP_CMPUSERBIPEDYOKE

#include "ICmpYoke.h"

namespace hesp {

/**
This class represents a yoke component that allows the user to control a biped.
Bipeds can walk, run, jump, crouch, etc.
*/
class CmpUserBipedYoke : public ICmpYoke
{
	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
};

}

#endif
