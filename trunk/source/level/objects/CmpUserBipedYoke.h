/***
 * hesperus: CmpUserBipedYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPUSERBIPEDYOKE
#define H_HESP_CMPUSERBIPEDYOKE

#include "ICmpYoke.h"

namespace hesp {

class CmpUserBipedYoke : public ICmpYoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	IYoke_Ptr m_yoke;

	//#################### STATIC FACTORY METHODS ####################
public:
	static IComponent_Ptr create(const Properties&);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets);
};

}

#endif
