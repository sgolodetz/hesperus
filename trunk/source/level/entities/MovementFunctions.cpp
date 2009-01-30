/***
 * hesperus: MovementFunctions.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MovementFunctions.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void MovementFunctions::move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, int milliseconds)
{
	// FIXME: Walking speed will eventually be a property of the entity.
	// FIXME: This will eventually take the navmesh etc. into account.
	const double WALK_SPEED = 5.0;
	ICameraComponent_Ptr camComponent = entity->camera_component();

	int mapIndex = 0;	// TEMPORARY
	const Vector3d& source = camComponent->camera().position();
	Vector3d dest = source + dir * WALK_SPEED * (milliseconds / 1000.0);

	OnionTree::Transition transition = tree->find_first_transition(mapIndex, source, dest);
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
			// Stop the player going into a wall.
			dest = *transition.location;
			break;
		}
		case OnionTree::RAY_TRANSITION_SE:
		{
			// We were in a wall prior to the move (this can happen very slightly due to the tolerance value
			// used when classifying a point against a plane) - better let the entity back into the world.
			break;
		}
	}

	camComponent->camera().set_position(dest);
}

void MovementFunctions::move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, int milliseconds)
{
	// NYI
	throw 23;
}

}
