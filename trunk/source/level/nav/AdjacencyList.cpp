/***
 * hesperus: AdjacencyList.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AdjacencyList.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
AdjacencyList::AdjacencyList(const NavMesh_Ptr& navMesh)
{
	const std::vector<NavLink_Ptr> links = navMesh->links();
	const std::vector<NavPolygon_Ptr> polygons = navMesh->polygons();

	m_size = static_cast<int>(links.size());
	m_adjacentEdges.resize(m_size);

	for(std::vector<NavPolygon_Ptr>::const_iterator it=polygons.begin(), iend=polygons.end(); it!=iend; ++it)
	{
		const std::vector<int>& linkIndices = (*it)->link_indices();
		size_t linkIndexCount = linkIndices.size();
		for(size_t j=0; j<linkIndexCount; ++j)
		{
			// TODO
		}
	}

	// NYI
	throw 23;
}

//#################### PUBLIC METHODS ####################
const std::list<AdjacencyList::Edge>& AdjacencyList::adjacent_edges(int node) const
{
	// NYI
	throw 23;
}

int AdjacencyList::size() const
{
	return m_size;
}

}
