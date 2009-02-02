/***
 * hesperus: BipedChangePoseCommand.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BipedChangePoseCommand.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BipedChangePoseCommand::BipedChangePoseCommand(const Entity_Ptr& biped)
:	m_biped(biped)
{}

//#################### PUBLIC METHODS ####################
void BipedChangePoseCommand::execute(const std::vector<AABB3d>& aabbs, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	// FIXME: Crouching is currently a "jolt" from one pose to another. It should really be a smooth transition.

	ICameraComponent_Ptr camComponent = m_biped->camera_component();
	ICollisionComponent_Ptr colComponent = m_biped->collision_component();

	Vector3d source = camComponent->camera().position();

	int curPose = colComponent->pose();
	int newPose = 1 - curPose;
	int curMapIndex = colComponent->aabb_indices()[curPose];
	int newMapIndex = colComponent->aabb_indices()[newPose];
	const AABB3d& curAABB = aabbs[curMapIndex];
	const AABB3d& newAABB = aabbs[newMapIndex];

	double deltaZ = newAABB.maximum().z - curAABB.maximum().z;

	Vector3d dest = source + Vector3d(0,0,deltaZ);

	// Check that changing pose won't put us in a wall.
	int destLeafIndex = tree->find_leaf_index(dest);
	const OnionLeaf *destLeaf = tree->leaf(destLeafIndex);
	if(!destLeaf->is_solid(newMapIndex))
	{
		// If the pose change is ok, set the new pose and update the entity position to reflect the centre of the new AABB.
		colComponent->set_pose(newPose);
		camComponent->camera().set_position(dest);
	}
}

}