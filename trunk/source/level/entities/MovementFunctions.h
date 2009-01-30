/***
 * hesperus: MovementFunctions.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MOVEMENTFUNCTIONS
#define H_HESP_MOVEMENTFUNCTIONS

#include <source/level/onionbsp/OnionTree.h>
#include <source/math/vectors/Vector3.h>
#include "Entity.h"

namespace hesp {

struct MovementFunctions
{
	//#################### PUBLIC METHODS ####################
	static void move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, int milliseconds);
	static void move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, int milliseconds);
};

}

#endif
