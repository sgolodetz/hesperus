/***
 * hesperus: CmpUserBipedYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpUserBipedYoke.h"

#include "YokeUserBiped.h"

namespace hesp {

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpUserBipedYoke::create(const Properties&)
{
	return IComponent_Ptr(new CmpUserBipedYoke);
}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> CmpUserBipedYoke::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons,
																   const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
{
	if(!m_yoke) m_yoke.reset(new YokeUserBiped(m_objectID, m_objectManager));
	return m_yoke->generate_commands(input, polygons, tree, navDatasets);
}

}
