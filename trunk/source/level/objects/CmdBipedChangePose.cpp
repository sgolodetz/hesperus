/***
 * hesperus: CmdBipedChangePose.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmdBipedChangePose.h"

#include <source/level/trees/TreeUtil.h>
#include "ICmpCollision.h"
#include "ICmpMeshMovement.h"
#include "ICmpPosition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmdBipedChangePose::CmdBipedChangePose(const ObjectID& objectID)
:	m_objectID(objectID)
{}

//#################### PUBLIC METHODS ####################
void CmdBipedChangePose::execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
								 const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	// FIXME: Crouching is currently a "jolt" from one pose to another. It should really be a smooth transition.

	ICmpCollision_Ptr cmpCollision = objectManager->get_component(m_objectID, cmpCollision);	assert(cmpCollision != NULL);
	ICmpMeshMovement_Ptr cmpMovement = objectManager->get_component(m_objectID, cmpMovement);	assert(cmpMovement != NULL);
	ICmpPosition_Ptr cmpPosition = objectManager->get_component(m_objectID, cmpPosition);		assert(cmpPosition != NULL);

	// Check that we're not currently traversing a nav link.
	if(cmpMovement->cur_traversal()) return;

	Vector3d source = cmpPosition->position();

	int curPose = cmpCollision->pose();
	int newPose = 1 - curPose;
	int curMapIndex = cmpCollision->aabb_indices()[curPose];
	int newMapIndex = cmpCollision->aabb_indices()[newPose];
	const AABB3d& curAABB = objectManager->aabbs()[curMapIndex];
	const AABB3d& newAABB = objectManager->aabbs()[newMapIndex];

	double deltaZ = newAABB.maximum().z - curAABB.maximum().z;

	Vector3d dest = source + Vector3d(0,0,deltaZ);

	// Check that changing pose won't put us in a wall.
	int destLeafIndex = TreeUtil::find_leaf_index(dest, tree);
	const OnionLeaf *destLeaf = tree->leaf(destLeafIndex);
	if(destLeaf->is_solid(newMapIndex)) return;

	// If the pose change is ok, set the new pose and update the entity position to reflect the centre of the new AABB.
	cmpCollision->set_pose(newPose);
	cmpPosition->set_position(dest);
	cmpMovement->set_cur_nav_poly_index(-1);
}

}
