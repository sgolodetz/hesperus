/***
 * hesperus: NavMeshGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_NAVMESHGENERATOR
#define H_HESP_LEVEL_NAV_NAVMESHGENERATOR

#include <source/level/onionbsp/OnionTree.h>
#include "NavPolygon.h"

namespace hesp {

class NavMeshGenerator
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	typedef std::vector<NavPolygon_Ptr> NavPolyVector;

	//#################### PRIVATE VARIABLES ####################
private:
	NavPolyVector m_polygons;
	OnionTree_Ptr m_tree;

	//#################### CONSTRUCTORS ####################
public:
	NavMeshGenerator(const ColPolyVector& polygons, const OnionTree_Ptr& tree);

	//#################### PRIVATE METHODS ####################
private:
	void build_polygon_to_leaf_map(std::vector<int>& polyToLeafMap, const OnionNode_Ptr& node);
};

}

#endif
