/***
 * hesperus: NavMeshGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavMeshGenerator.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavMeshGenerator::NavMeshGenerator(const ColPolyVector& polygons, const OnionTree_Ptr& tree)
:	m_tree(tree)
{
	int polyCount = static_cast<int>(polygons.size());

	// Traverse the tree and build a map from polygons -> leaf indices.
	std::vector<int> polyToLeafMap(polyCount);
	build_polygon_to_leaf_map(polyToLeafMap, tree->root());

	// Convert all the collision polygons into navigation polygons. Store NULL in the base array
	// for those that aren't walkable. Note that we have to keep the base array the same size
	// because otherwise the polygon indices in the tree wouldn't be valid any more. To make up
	// for this, we maintain a separate "walkable polygons" array which we use when actually
	// building the navigation mesh.
	m_polygons.resize(polyCount);
	for(int i=0; i<polyCount; ++i)
	{
		if(polygons[i]->auxiliary_data().walkable())
		{
			NavPolyAuxData auxData(polygons[i]->auxiliary_data(), polyToLeafMap[i]);
			m_polygons[i].reset(new NavPolygon(*polygons[i], auxData));
			m_walkablePolygons.push_back(m_polygons[i]);
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
		// TODO
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

}
