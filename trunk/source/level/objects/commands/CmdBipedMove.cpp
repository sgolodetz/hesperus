/***
 * hesperus: CmdBipedMove.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmdBipedMove.h"

#include <source/level/objects/MoveFunctions.h>
#include <source/level/objects/components/ICmpBipedAnimChooser.h>

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

	// FIXME: This will eventually need to be changed to set either the walk or run flag (whichever is appropriate).
	ICmpBipedAnimChooser_Ptr cmpAnimChooser = objectManager->get_component(m_objectID, cmpAnimChooser);
	if(cmpAnimChooser) cmpAnimChooser->set_walk_flag();
}

}
