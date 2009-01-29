/***
 * hesperus: MovementFunctions.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MOVEMENTFUNCTIONS
#define H_HESP_MOVEMENTFUNCTIONS

#include <source/math/vectors/Vector3.h>
#include "Entity.h"

namespace hesp {

struct MovementFunctions
{
	//#################### PUBLIC METHODS ####################
	static void move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, int milliseconds/* TODO */);
	static void move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, int milliseconds/* TODO */);
};

}

#endif
