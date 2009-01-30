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

class MovementFunctions
{
	//#################### NESTED CLASSES ####################
private:
	struct Move
	{
		double timeRemaining;	// the time remaining for moving (in seconds)
		Vector3d dir;			// the current desired movement direction
		int mapIndex;			// which navigation map the entity's on
	};

	//#################### PUBLIC METHODS ####################
public:
	static void move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, int milliseconds);
	static void move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, const OnionTree_Ptr& tree, int milliseconds);

	//#################### PRIVATE METHODS ####################
private:
	static void do_direct_move(const Entity_Ptr& entity, Move& move, const OnionTree_Ptr& tree);
};

}

#endif
