/***
 * hesperus: NavDataSet.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVDATASET
#define H_HESP_NAVDATASET

#include "AdjacencyList.h"
#include "NavMesh.h"
#include "PathTable.h"

namespace hesp {

/**
An instance of this class stores all the necessary nav data for a particular AABB map.
*/
class NavDataset
{
	//#################### PRIVATE VARIABLES ####################
private:
	AdjacencyList_Ptr m_adjList;
	NavMesh_Ptr m_navMesh;
	PathTable_Ptr m_pathTable;

	//#################### CONSTRUCTORS ####################
public:
	NavDataset(const AdjacencyList_Ptr& adjList, const NavMesh_Ptr& navMesh, const PathTable_Ptr& pathTable)
	:	m_adjList(adjList), m_navMesh(navMesh), m_pathTable(pathTable)
	{}

	//#################### PUBLIC METHODS ####################
public:
	const AdjacencyList_Ptr& adjacency_list() const		{ return m_adjList; }
	const NavMesh_Ptr& nav_mesh() const					{ return m_navMesh; }
	const PathTable_Ptr& path_table() const				{ return m_pathTable; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavDataset> NavDataset_Ptr;
typedef shared_ptr<const NavDataset> NavDataset_CPtr;

}

#endif
