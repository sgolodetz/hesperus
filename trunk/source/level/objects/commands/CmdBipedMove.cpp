/***
 * hesperus: CmdBipedMove.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmdBipedMove.h"

#include <source/level/objects/MoveFunctions.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmdBipedMove::CmdBipedMove(const ObjectID& objectID, const Vector3d& dir, double speed)
:	m_objectID(objectID), m_dir(dir), m_speed(speed)
{}

//#################### PUBLIC METHODS ####################
void CmdBipedMove::execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
						   const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	MoveFunctions::move_with_navmesh(m_objectID, objectManager, m_dir, m_speed, polygons, tree, navDatasets, milliseconds);
}

}
