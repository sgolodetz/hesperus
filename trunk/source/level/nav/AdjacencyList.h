/***
 * hesperus: AdjacencyList.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_ADJACENCYLIST
#define H_HESP_LEVEL_NAV_ADJACENCYLIST

#include <list>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "NavMesh.h"

namespace hesp {

/**
This class provides an adjacency list representation of a +ve-weighted,
directed graph. This allows us to quickly retrieve the list of edges
leading out of a given node, without requiring copious amounts of
storage space.
*/
class AdjacencyList
{
	//#################### NESTED CLASSES ####################
public:
	class Edge
	{
		//#################### PRIVATE VARIABLES ####################
	private:
		int m_toNode;
		float m_length;
	public:
		//#################### CONSTRUCTORS ####################
		Edge(int toNode, float length)
		:	m_toNode(toNode), m_length(length)
		{}

		//#################### PUBLIC METHODS ####################
	public:
		float length() const	{ return m_length; }
		int to_node() const		{ return m_toNode; }
	};

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<std::list<Edge> > m_adjacentEdges;
	int m_size;

	//#################### CONSTRUCTORS ####################
public:
	AdjacencyList(const NavMesh_Ptr& navMesh);

	//#################### PUBLIC METHODS ####################
public:
	const std::list<Edge>& adjacent_edges(int node) const;
	int size() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<AdjacencyList> AdjacencyList_Ptr;
typedef shared_ptr<const AdjacencyList> AdjacencyList_CPtr;

}

#endif
