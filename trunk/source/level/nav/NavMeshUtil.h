/***
 * hesperus: NavMeshUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVMESHUTIL
#define H_HESP_NAVMESHUTIL

#include <source/level/trees/OnionTree.h>
#include <source/util/PolygonTypes.h>
#include "NavMesh.h"

namespace hesp {

struct NavMeshUtil
{
	static int find_nav_polygon(const Vector3d& p, int suggestedNavPoly, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavMesh_CPtr& navMesh);
};

}

#endif
