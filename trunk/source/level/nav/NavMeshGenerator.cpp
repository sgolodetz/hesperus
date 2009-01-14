/***
 * hesperus: NavMeshGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavMeshGenerator.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavMeshGenerator::NavMeshGenerator(const ColPolyVector& polygons, const OnionTree_Ptr& tree)
{
	// Traverse the tree and build a map from polygons -> leaf indices.
	// TODO

	// Convert all the collision polygons into navigation polygons. Store NULL in the array
	// for those that aren't walkable. Note that we have to keep the arrays the same size
	// because otherwise the polygon indices in the tree wouldn't be valid any more.
	for(ColPolyVector::const_iterator it=polygons.begin(), iend=polygons.end(); it!=iend; ++it)
	{
		// TODO
	}
}

}
