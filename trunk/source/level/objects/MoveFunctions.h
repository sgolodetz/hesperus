/***
 * hesperus: MoveFunctions.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MOVEFUNCTIONS
#define H_HESP_MOVEFUNCTIONS

#include <source/level/nav/NavDataset.h>
#include <source/level/objects/base/ObjectManager.h>
#include <source/level/trees/OnionTree.h>
#include <source/math/vectors/Vector3.h>

namespace hesp {

class MoveFunctions
{
	//#################### NESTED CLASSES ####################
private:
	struct Move
	{
		double timeRemaining;	// the time remaining for moving (in seconds)
		Vector3d dir;			// the current desired movement direction
		int mapIndex;			// which navigation map the object's on
	};

	//#################### PUBLIC METHODS ####################
public:
	static bool attempt_navmesh_acquisition(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const NavMesh_Ptr& navMesh);
	static int find_nav_polygon(const Vector3d& p, int suggestedNavPoly, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const NavMesh_Ptr& navMesh);
	static void move_with_navmesh(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, const Vector3d& dir, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds);
	static bool single_move_without_navmesh(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, const Vector3d& dir, double speed, const OnionTree_Ptr& tree, int milliseconds);

	//#################### PRIVATE METHODS ####################
private:
	static bool do_direct_move(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, Move& move, double speed, const OnionTree_Ptr& tree);
	static void do_navmesh_move(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, Move& move, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const NavMesh_Ptr& navMesh);
	static void do_traverse_move(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, Move& move, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const NavMesh_Ptr& navMesh);
	static void update_move_direction_for_sliding(const ObjectID& objectID, const ObjectManager_Ptr& objectManager, Move& move);
};

}

#endif
