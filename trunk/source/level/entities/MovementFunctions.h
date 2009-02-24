/***
 * hesperus: MovementFunctions.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MOVEMENTFUNCTIONS
#define H_HESP_MOVEMENTFUNCTIONS

#include <source/level/onionbsp/OnionTree.h>
#include <source/level/nav/NavDataset.h>
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
	static bool attempt_navmesh_acquisition(const Entity_Ptr& entity, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const NavMesh_Ptr& navMesh);
	static int find_nav_polygon(const Vector3d& p, int suggestedNavPoly, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const NavMesh_Ptr& navMesh);
	static void move_with_navmesh(const Entity_Ptr& entity, const Vector3d& dir, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds);
	static bool single_move_without_navmesh(const Entity_Ptr& entity, const Vector3d& dir, double speed, const OnionTree_Ptr& tree, int milliseconds);

	//#################### PRIVATE METHODS ####################
private:
	static bool do_direct_move(const Entity_Ptr& entity, Move& move, double speed, const OnionTree_Ptr& tree);
	static void do_navmesh_move(const Entity_Ptr& entity, Move& move, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const NavMesh_Ptr& navMesh);
	static void do_traverse_move(const Entity_Ptr& entity, Move& move, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const NavMesh_Ptr& navMesh);
	static void update_move_direction_for_sliding(const Entity_Ptr& entity, Move& move);
};

}

#endif
