/***
 * hesperus: NavMeshGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavMeshGenerator.h"

#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
NavMeshGenerator::NavMeshGenerator(const ColPolyVector& polygons, const OnionTree_Ptr& tree)
:	m_polygons(polygons), m_tree(tree), m_edgePlaneTable(UniquePlanePred(2 * PI/180, 0.005))
{
	int polyCount = static_cast<int>(polygons.size());

	// Traverse the tree and build a map from polygons -> leaf indices.
	std::vector<int> polyToLeafMap(polyCount);
	build_polygon_to_leaf_map(polyToLeafMap, tree->root());

	// Construct the walkable polygon array.
	m_colToNavMap.resize(polyCount, -1);
	for(int i=0; i<polyCount; ++i)
	{
		if(m_polygons[i]->auxiliary_data().walkable())
		{
			m_colToNavMap[i] = static_cast<int>(m_walkablePolygons.size());
			m_walkablePolygons.push_back(NavPolygon_Ptr(new NavPolygon(i, polyToLeafMap[i])));
		}
	}
}

//#################### PUBLIC METHODS ####################
void NavMeshGenerator::generate_mesh()
{
	build_edge_plane_table();
	// TODO
}

//#################### PRIVATE METHODS ####################
void NavMeshGenerator::build_edge_plane_table()
{
	int walkablePolyCount = static_cast<int>(m_walkablePolygons.size());

	for(int i=0; i<walkablePolyCount; ++i)
	{
		const CollisionPolygon& curPoly = *m_polygons[m_walkablePolygons[i]->poly_index()];
		int vertCount = curPoly.vertex_count();
		for(int j=0; j<vertCount; ++j)
		{
			int k = (j+1)%vertCount;
			const Vector3d& p1 = curPoly.vertex(j);
			const Vector3d& p2 = curPoly.vertex(k);
			m_edgePlaneTable[make_edge_plane(p1,p2)].push_back(EdgeReference(i,j));
		}
	}
}

void NavMeshGenerator::build_polygon_to_leaf_map(std::vector<int>& polyToLeafMap, const OnionNode_Ptr& node)
{
	if(node->is_leaf())
	{
		OnionLeaf *leaf = node->as_leaf();
		int leafIndex = leaf->leaf_index();
		const std::vector<int>& polyIndices = leaf->polygon_indices();
		int polyCount = static_cast<int>(polyIndices.size());
		for(int i=0; i<polyCount; ++i)
		{
			polyToLeafMap[polyIndices[i]] = leafIndex;
		}
	}
	else
	{
		OnionBranch *branch = node->as_branch();
		build_polygon_to_leaf_map(polyToLeafMap, branch->left());
		build_polygon_to_leaf_map(polyToLeafMap, branch->right());
	}
}

Plane NavMeshGenerator::make_edge_plane(const Vector3d& p1, const Vector3d& p2)
{
	// TODO:	This is very similar to BrushExpander::make_bevel_plane(). It's a good idea to
	//			factor out the commonality at some point.
	Vector3d u = (p2 - p1).normalize();
	Vector3d v(0,0,1);
	Vector3d n = u.cross(v);

	if(n.length_squared() < EPSILON*EPSILON)
		throw Exception("Bad input to the navigation mesh generator: one of the polygons was vertical");

	return Plane(n, p1).to_undirected_form();
}

}
