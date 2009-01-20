/***
 * hesperus: PathTableGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PATHTABLEGENERATOR
#define H_HESP_PATHTABLEGENERATOR

#include "AdjacencyTable.h"
#include "PathTable.h"

namespace hesp {

class PathTableGenerator
{
	//#################### PUBLIC METHODS ####################
public:
	static PathTable_Ptr floyd_warshall(const AdjacencyTable& adjTable);
};

}

#endif
