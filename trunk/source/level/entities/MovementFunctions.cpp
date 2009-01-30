/***
 * hesperus: MovementFunctions.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MovementFunctions.h"

#include <source/math/Constants.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
void MovementFunctions::move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, int milliseconds)
{
	// FIXME: This will eventually take the navmesh etc. into account.
	move_without_navmesh(entity, dir, tree, milliseconds);
}

void MovementFunctions::move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, int milliseconds)
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
void MovementFunctions::do_direct_move(const Entity_Ptr& entity, Move& move, const OnionTree_Ptr& tree)
{
	// FIXME: Walking speed will eventually be a property of the entity.
	const double WALK_SPEED = 5.0;	// in units/s
	ICameraComponent_Ptr camComponent = entity->camera_component();

	const Vector3d& source = camComponent->camera().position();
	Vector3d dest = source + move.dir * WALK_SPEED * move.timeRemaining;

	// Check the ray against the tree (adding in a bit of tolerance to make it detect "not quite" collisions early).
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
			// We were in a wall prior to the move (this can happen very slightly due to the tolerance value
			// used when classifying a point against a plane) - prevent any further moves into the wall.
			return;
		}
		case OnionTree::RAY_TRANSITION_ES:
		{
			// Stop the entity going into a wall (we don't put the entity right on the wall - no point in making life hard for our classification routines).
			dest = *transition.location + transition.plane->normal() * 0.02;

			// Update the move direction to be along the wall (to allow sliding). To do this, we remove the
			// component of the movement which is normal to the wall.
			Vector3d normalComponent = move.dir.project_onto(transition.plane->normal());
			move.dir -= normalComponent;
			if(move.dir.length() > SMALL_EPSILON) move.dir.normalize();
			else move.timeRemaining = 0;

			break;
		}
		case OnionTree::RAY_TRANSITION_SE:
		{
			// We were in a wall prior to the move (this can happen very slightly due to the tolerance value
			// used when classifying a point against a plane) - better let the entity back into the world.
			break;
		}
	}

	// Update the time remaining.
	double moveLength = source.distance(dest);
	double timeTaken = moveLength / WALK_SPEED;
	move.timeRemaining -= timeTaken;

	camComponent->camera().set_position(dest);
}

}
