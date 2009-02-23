/***
 * hesperus: NullYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NULLYOKE
#define H_HESP_NULLYOKE

#include <source/level/entities/EntityCommand.h>
#include <source/level/entities/Yoke.h>

namespace hesp {

/**
This class represents a yoke which generates no entity commands.
*/
class NullYoke : public Yoke
{
	//#################### PUBLIC METHODS ####################
public:
	std::vector<EntityCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
	{
		return std::vector<EntityCommand_Ptr>();
	}
};

}

#endif
