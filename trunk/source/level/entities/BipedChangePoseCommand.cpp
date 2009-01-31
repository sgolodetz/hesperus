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
void BipedChangePoseCommand::execute(const std::vector<AABB3d>& aabbs, const OnionTree_Ptr& tree, int milliseconds)
{
	ICameraComponent_Ptr camComponent = m_biped->camera_component();
	ICollisionComponent_Ptr colComponent = m_biped->collision_component();

	Vector3d source = camComponent->camera().position();

	int curPose = colComponent->pose();
	int newPose = 1 - curPose;
	const AABB3d& curAABB = aabbs[colComponent->aabb_indices()[curPose]];
	const AABB3d& newAABB = aabbs[colComponent->aabb_indices()[newPose]];

	// Note: This can obviously be easily "optimized", but it's clearer to write it like this.
	double deltaZ = (newAABB.maximum().z + newAABB.minimum().z) / 2 - (curAABB.maximum().z + curAABB.minimum().z) / 2;

	Vector3d dest = source + Vector3d(0,0,deltaZ);

	OnionTree::Transition transition = tree->find_first_transition(curPose, source, dest);
	if(transition.classifier != OnionTree::RAY_EMPTY) return;

	transition = tree->find_first_transition(newPose, source, dest);
	if(transition.classifier != OnionTree::RAY_EMPTY) return;

	colComponent->set_pose(newPose);
	camComponent->camera().set_position(dest);
}

}
