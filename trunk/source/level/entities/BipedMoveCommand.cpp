/***
 * hesperus: BipedMoveCommand.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BipedMoveCommand.h"
#include "MovementFunctions.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BipedMoveCommand::BipedMoveCommand(const Entity_Ptr& biped, const Vector3d& dir, double speed)
:	m_biped(biped), m_dir(dir), m_speed(speed)
{}

//#################### PUBLIC METHODS ####################
void BipedMoveCommand::execute(const std::vector<AABB3d>& aabbs, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
							   const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	MovementFunctions::move_with_navmesh(m_biped, m_dir, m_speed, polygons, tree, navDatasets, milliseconds);
}

}
