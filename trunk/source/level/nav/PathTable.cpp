/***
 * hesperus: PathTable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PathTable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
PathTable::PathTable(int size)
:	m_table(size), m_size(size)
{
	for(int i=0; i<size; ++i)
	{
		m_table[i].resize(size);
		m_table[i][i].cost = 0.0f;
	}
}

//#################### PUBLIC METHODS ####################
float& PathTable::cost(int i, int j)			{ return m_table[i][j].cost; }
float PathTable::cost(int i, int j) const		{ return m_table[i][j].cost; }
int& PathTable::next_node(int i, int j)			{ return m_table[i][j].nextNode; }
int PathTable::next_node(int i, int j) const	{ return m_table[i][j].nextNode; }
int PathTable::size() const						{ return m_size; }

}
