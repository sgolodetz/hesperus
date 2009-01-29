/***
 * hesperus: MovementFunctions.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MovementFunctions.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void MovementFunctions::move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, int milliseconds/* TODO */)
{
	// FIXME: Walking speed will eventually be a property of the entity.
	// FIXME: This will eventually call an external function to move the object properly (i.e. taking the nav mesh and collisions into account).
	const double WALK_SPEED = 5.0;
	ICameraComponent_Ptr camComponent = entity->camera_component();
	camComponent->camera().set_position(camComponent->camera().position() + dir * WALK_SPEED * (milliseconds / 1000.0));
}

void MovementFunctions::move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, int milliseconds/* TODO */)
{
	// NYI
	throw 23;
}

}
