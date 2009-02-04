/***
 * hesperus: BipedJumpCommand.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BipedJumpCommand.h"

#include "MovementFunctions.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BipedJumpCommand::BipedJumpCommand(const Entity_Ptr& biped, const Vector3d& dir)
:	m_biped(biped), m_dir(dir)
{}

//#################### PUBLIC METHODS ####################
void BipedJumpCommand::execute(const std::vector<AABB3d>& aabbs, const std::vector<CollisionPolygon_Ptr>& polygons,
							   const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	ICollisionComponent_Ptr colComponent = m_biped->collision_component();
	int mapIndex = colComponent->aabb_indices()[colComponent->pose()];
	NavMesh_Ptr navMesh = navDatasets[mapIndex]->nav_mesh();

	if(MovementFunctions::attempt_navmesh_acquisition(m_biped, polygons, tree, navMesh))
	{
		IPhysicsComponent_Ptr physComponent = m_biped->physics_component();

		// FIXME: The jump strength should eventually be a property of the entity.
		const double JUMP_STRENGTH = 3;	// force of jump in Newtons
		Vector3d velocity = m_dir + Vector3d(0,0,1) * JUMP_STRENGTH;
		physComponent->set_velocity(velocity);
	}
}

}
