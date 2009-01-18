/***
 * hesperus: GlobalPathfinder.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GlobalPathfinder.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
GlobalPathfinder::GlobalPathfinder(const NavMesh_Ptr& navMesh, const AdjacencyList_Ptr& adjList,
								   const PathTable_Ptr& pathTable)
:	m_navMesh(navMesh), m_adjList(adjList), m_pathTable(pathTable)
{}

//#################### PUBLIC METHODS ####################
/**
Tries to find a (high-level) path from sourcePos in sourcePoly to destPos in destPoly.

@param sourcePos	The source position
@param sourcePoly	The source nav polygon
@param destPos		The destination position
@param destPoly		The destination nav polygon
@param path			Used to return the path (if found) to the caller
@return				true, if a path was found, or false otherwise
*/
bool GlobalPathfinder::find_path(const Vector3d& sourcePos, int sourcePoly,
								 const Vector3d& destPos, int destPoly,
								 std::list<int>& path) const
{
	// Step 1:	Find any unblocked path-table paths from a source navlink to a dest navlink.
	// TODO

	// Step 2:	If any unblocked path-table paths exist, pick the shortest one.
	// TODO

	// Step 3:	If no unblocked path-table paths exist, do an A* search on the adjacency list
	//			representation of the navigation graph. Use a temporary node in both the source
	//			and destination polygons to represent the actual position of the player.
	// TODO

	// Step 4:	If an A* path was found, use it. Otherwise, no valid path exists.
	// TODO

	// NYI
	throw 23;
}

}
