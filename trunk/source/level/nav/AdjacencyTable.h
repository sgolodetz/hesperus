/***
 * hesperus: AdjacencyTable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_ADJACENCYTABLE
#define H_HESP_LEVEL_NAV_ADJACENCYTABLE

#include "AdjacencyList.h"

namespace hesp {

class AdjacencyTable
{
	//#################### CONSTRUCTORS ####################
public:
	AdjacencyTable(const AdjacencyList& adjList);

	//#################### PUBLIC OPERATORS ####################
public:
	float operator()(int i, int j) const;

	//#################### PUBLIC METHODS ####################
public:
	int size() const;
};

}

#endif
