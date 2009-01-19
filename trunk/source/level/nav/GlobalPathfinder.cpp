/***
 * hesperus: GlobalPathfinder.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GlobalPathfinder.h"

#include <queue>

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
	// Step 1:	Find the shortest unblocked path-table path from a source navlink to a dest navlink.
	//			If such a path was found, use it.
	const std::vector<NavLink_Ptr>& links = m_navMesh->links();
	const std::vector<NavPolygon_Ptr>& polygons = m_navMesh->polygons();
	const std::vector<int>& sourceLinkIndices = polygons[sourcePoly]->out_links();
	const std::vector<int>& destLinkIndices = polygons[destPoly]->in_links();

	// Work out the costs of all the possible paths and get them ready to be processed in ascending order of cost.
	std::priority_queue<PathDescriptor, std::vector<PathDescriptor>, std::greater<PathDescriptor> > pq;
	int sourceLinkCount = static_cast<int>(sourceLinkIndices.size());
	int destLinkCount = static_cast<int>(destLinkIndices.size());
	for(int i=0; i<sourceLinkCount; ++i)
	{
		int sourceLinkIndex = sourceLinkIndices[i];
		const NavLink_Ptr& sourceLink = links[sourceLinkIndex];
		float sourceCost = static_cast<float>(sourcePos.distance(sourceLink->source_position()));

		for(int j=0; j<destLinkCount; ++j)
		{
			// The cost of going from sourcePos to destPos via the shortest path-table path
			// between the two navlinks is the sum of the cost of going to the source navlink,
			// the cost of going from the source navlink to the dest navlink, and the cost of
			// going from the dest navlink to destPos.
			int destLinkIndex = destLinkIndices[j];
			const NavLink_Ptr& destLink = links[destLinkIndex];
			float destCost = static_cast<float>(destPos.distance(destLink->dest_position()));
			float interlinkCost = m_pathTable->cost(sourceLinkIndex, destLinkIndex);
			pq.push(PathDescriptor(sourceCost + interlinkCost + destCost, sourceLinkIndex, destLinkIndex));
		}
	}

	// Started from the least costly path, construct it and see whether it's blocked or not. If not, use it.
	while(!pq.empty())
	{
		PathDescriptor desc = pq.top();
		pq.pop();

		path = m_pathTable->construct_path(desc.sourceLink, desc.destLink);

		if(!is_blocked(sourcePos, path, destPos))
			return true;	// note that the path to be returned has already been stored in the out parameter
	}

	// Step 2:	If no unblocked path-table paths exist, do an A* search on the adjacency list
	//			representation of the navigation graph. Use a temporary node in both the source
	//			and destination polygons to represent the actual position of the player.
	// TODO

	// Step 3:	If an A* path was found, use it. Otherwise, no valid path exists.
	// TODO

	// NYI
	return false;
}

//#################### PRIVATE METHODS ####################
bool GlobalPathfinder::is_blocked(const Vector3d& sourcePos, const std::list<int>& potentialPath, const Vector3d& destPos) const
{
	// NYI
	return false;
}

}
