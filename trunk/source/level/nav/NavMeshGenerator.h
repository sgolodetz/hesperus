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
	// Input data
	OnionTree_CPtr m_tree;

	// Intermediate/output data
	NavPolyVector m_polygons;
	NavPolyVector m_walkablePolygons;

	//#################### CONSTRUCTORS ####################
public:
	NavMeshGenerator(const ColPolyVector& polygons, const OnionTree_Ptr& tree);

	//#################### PUBLIC METHODS ####################
public:
	void generate_mesh();

	//#################### PRIVATE METHODS ####################
private:
	void build_edge_plane_table();
	void build_polygon_to_leaf_map(std::vector<int>& polyToLeafMap, const OnionNode_Ptr& node);
};

}

#endif
