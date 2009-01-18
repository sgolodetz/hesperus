/***
 * hesperus: AdjacencyList.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_ADJACENCYLIST
#define H_HESP_LEVEL_NAV_ADJACENCYLIST

#include <list>

#include "NavMesh.h"

namespace hesp {

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

	//#################### CONSTRUCTORS ####################
public:
	AdjacencyList(const NavMesh_Ptr& navMesh);

	//#################### PUBLIC METHODS ####################
public:
	const std::list<Edge>& adjacent_edges(int node) const;
	int size() const;
};

}

#endif
