/***
 * hesperus: MovementFunctions.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MovementFunctions.h"

#if 1
#include <iostream>
#endif

#include <source/math/Constants.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
void MovementFunctions::move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	// FIXME: This will eventually take the navmesh etc. into account.

	INavComponent_Ptr navComponent = entity->nav_component();
#if 0
	std::cout << navComponent->cur_nav_poly_index() << std::endl;
#endif
	Move move;
	move.dir = dir;
	move.mapIndex = entity->collision_component()->pose();
	move.timeRemaining = milliseconds / 1000.0;

	double oldTimeRemaining;
	do
	{
		oldTimeRemaining = move.timeRemaining;
		if(attempt_navmesh_acquisition(entity, tree, navDatasets)) { /* NYI */ }
		else do_direct_move(entity, move, tree);
	} while(move.timeRemaining > 0.0005 && oldTimeRemaining - move.timeRemaining > 0.0001);
}

void MovementFunctions::move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	Move move;
	move.dir = dir;
	move.mapIndex = entity->collision_component()->pose();
	move.timeRemaining = milliseconds / 1000.0;

	double oldTimeRemaining;
	do
	{
		oldTimeRemaining = move.timeRemaining;
		do_direct_move(entity, move, tree);
	} while(move.timeRemaining > 0.0005 && oldTimeRemaining - move.timeRemaining > 0.0001);
}

//#################### PRIVATE METHODS ####################
bool MovementFunctions::attempt_navmesh_acquisition(const Entity_Ptr& entity, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets)
{
	return false;
}

void MovementFunctions::do_direct_move(const Entity_Ptr& entity, Move& move, const OnionTree_Ptr& tree)
{
	// FIXME: Walking speed will eventually be a property of the entity.
	const double WALK_SPEED = 5.0;	// in units/s
	ICameraComponent_Ptr camComponent = entity->camera_component();
	ICollisionComponent_Ptr colComponent = entity->collision_component();

	const Vector3d& source = camComponent->camera().position();
	Vector3d dest = source + move.dir * WALK_SPEED * move.timeRemaining;

	// Check the ray against the tree.
	OnionTree::Transition transition = tree->find_first_transition(move.mapIndex, source, dest);
	switch(transition.classifier)
	{
		case OnionTree::RAY_EMPTY:
		{
			// It's perfectly fine to let the entity move along this ray, as it doesn't intersect a wall.
			break;
		}
		case OnionTree::RAY_SOLID:
		{
			// We were on a wall (i.e. coplanar to it) prior to the move - prevent any further moves into the wall,
			// and update the move direction to allow sliding along the wall instead.
			update_move_direction_for_sliding(entity, move);
			move.timeRemaining -= 0.001;	// make this cost 1ms of time (otherwise the calling function will think we got stuck)
			return;
		}
		case OnionTree::RAY_TRANSITION_ES:
		{
			// Stop the entity going into a wall.
			dest = *transition.location;

			// Record this as the latest transition.
			colComponent->update_last_transition(OnionTree::Transition_Ptr(new OnionTree::Transition(transition)));

			// Update the move direction to allow sliding.
			update_move_direction_for_sliding(entity, move);

			break;
		}
		case OnionTree::RAY_TRANSITION_SE:
		{
			// This should never happen (since entities can't move into walls), but better let the entity back into
			// the world if it does happen.
			break;
		}
	}

	// Update the time remaining.
	double moveLength = source.distance(dest);
	double timeTaken = moveLength / WALK_SPEED;
	move.timeRemaining -= timeTaken;

	camComponent->camera().set_position(dest);
}

void MovementFunctions::update_move_direction_for_sliding(const Entity_Ptr& entity, Move& move)
{
	// Update the move direction to be along the wall (to allow sliding). To do this, we remove the
	// component of the movement which is normal to the wall.

	ICollisionComponent_Ptr colComponent = entity->collision_component();
	const OnionTree::Transition_Ptr& transition = colComponent->last_transition();

	if(transition)
	{
		Vector3d normalComponent = move.dir.project_onto(transition->plane->normal());
		move.dir -= normalComponent;
		if(move.dir.length() > SMALL_EPSILON) move.dir.normalize();
		else move.timeRemaining = 0;
	}
}

}
