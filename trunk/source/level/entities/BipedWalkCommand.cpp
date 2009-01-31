/***
 * hesperus: BipedWalkCommand.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BipedWalkCommand.h"
#include "MovementFunctions.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BipedWalkCommand::BipedWalkCommand(const Entity_Ptr& biped, const Vector3d& dir)
:	m_biped(biped), m_dir(dir)
{}

//#################### PUBLIC METHODS ####################
void BipedWalkCommand::execute(const std::vector<AABB3d>& aabbs, const OnionTree_Ptr& tree, int milliseconds)
{
	MovementFunctions::move_with_navmesh(m_biped, m_dir, tree, milliseconds);
}

}
