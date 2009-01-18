/***
 * hesperus: GlobalPathfinder.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_GLOBALPATHFINDER
#define H_HESP_LEVEL_NAV_GLOBALPATHFINDER

#include "AdjacencyList.h"
#include "PathTable.h"

namespace hesp {

/**
This class provides global (high-level) pathfinding, i.e.
it allows us to find a path from one side of the level to
the other. There should be one of these for each navmesh,
i.e. one for each AABB map.
*/
class GlobalPathfinder
{
	//#################### NESTED CLASSES ####################
private:
	struct PathDescriptor
	{
		float cost;
		int sourceLink;
		int destLink;

		PathDescriptor(float cost_, int sourceLink_, int destLink_)
		:	cost(cost_), sourceLink(sourceLink_), destLink(destLink_)
		{}

		bool operator<(const PathDescriptor& rhs) const
		{
			return cost < rhs.cost;
		}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	NavMesh_CPtr m_navMesh;
	AdjacencyList_Ptr m_adjList;
	PathTable_CPtr m_pathTable;

	//#################### CONSTRUCTORS ####################
public:
	GlobalPathfinder(const NavMesh_Ptr& navMesh, const AdjacencyList_Ptr& adjList, const PathTable_Ptr& pathTable);

	//#################### PUBLIC METHODS ####################
public:
	bool find_path(const Vector3d& sourcePos, int sourcePoly, const Vector3d& destPos, int destPoly, std::list<int>& path) const;

	//#################### PRIVATE METHODS ####################
private:
	bool is_blocked(const Vector3d& sourcePos, const std::list<int>& potentialPath, const Vector3d& destPos) const;
};

}

#endif
