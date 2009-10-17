/***
 * hesperus: CmpUserBipedYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpUserBipedYoke.h"

#include <source/exceptions/Exception.h>
#include <source/level/objects/components/ICmpBounds.h>
#include <source/level/objects/components/ICmpMeshMovement.h>
#include <source/level/objects/components/ICmpOrientation.h>
#include <source/level/objects/components/ICmpSimulation.h>
#include <source/level/objects/yokes/user/UserBipedYoke.h>
#include <source/util/Properties.h>

namespace hesp {

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpUserBipedYoke::load(const Properties&)
{
	return IObjectComponent_Ptr(new CmpUserBipedYoke);
}

//#################### PUBLIC METHODS ####################
void CmpUserBipedYoke::check_dependencies() const
{
	check_dependency<ICmpBounds>();
	check_dependency<ICmpMeshMovement>();
	check_dependency<ICmpOrientation>();
	check_dependency<ICmpSimulation>();
}

std::vector<ObjectCommand_Ptr> CmpUserBipedYoke::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons,
																   const OnionTree_CPtr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
{
	if(!m_yoke) m_yoke.reset(new UserBipedYoke(m_objectID, m_objectManager));
	return m_yoke->generate_commands(input, polygons, tree, navDatasets);
}

Properties CmpUserBipedYoke::save() const
{
	return Properties();
}

}
