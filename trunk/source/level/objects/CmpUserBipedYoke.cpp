/***
 * hesperus: CmpUserBipedYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpUserBipedYoke.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> CmpUserBipedYoke::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
																   const std::vector<NavDataset_Ptr>& navDatasets)
{
	// FIXME: The key mappings should be defined externally, not hard-coded like this.

	std::vector<ObjectCommand_Ptr> commands;

	// NYI
	throw 23;

	return commands;
}

}
