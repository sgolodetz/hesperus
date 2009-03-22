/***
 * hesperus: NavSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVSECTION
#define H_HESP_NAVSECTION

#include <source/level/nav/NavDataset.h>

namespace hesp {

class NavSection
{
	//#################### LOADING METHODS ####################
public:
	static std::vector<NavDataset_Ptr> load(std::istream& is);

	//#################### SAVING METHODS ####################
public:
	static void save(std::ostream& os, const std::vector<NavDataset_Ptr>& datasets);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static AdjacencyList_Ptr read_adjacency_list(std::istream& is);
	static NavMesh_Ptr read_navmesh(std::istream& is);
	static PathTable_Ptr read_path_table(std::istream& is);

	//#################### SAVING SUPPORT METHODS ####################
private:
	static void write_adjacency_list(std::ostream& os, const AdjacencyList_Ptr& adjList);
	static void write_navmesh(std::ostream& os, const NavMesh_Ptr& mesh);
	static void write_path_table(std::ostream& os, const PathTable_Ptr& pathTable);
};

}

#endif
