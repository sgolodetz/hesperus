/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

#include <source/level/nav/NavDataset.h>
#include <source/level/nav/NavMesh.h>
#include <source/level/trees/BSPTree.h>
#include <source/level/trees/TreeUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Level::Level(const GeometryRenderer_Ptr& geomRenderer, const BSPTree_Ptr& tree,
			 const PortalVector& portals, const LeafVisTable_Ptr& leafVis,
			 const ColPolyVector& onionPolygons, const OnionTree_Ptr& onionTree,
			 const OnionPortalVector& onionPortals, const NavDatasetVector& navDatasets,
			 const ObjectManager_Ptr& objectManager, const ModelManager_Ptr& modelManager)
:	m_geomRenderer(geomRenderer), m_tree(tree), m_portals(portals), m_leafVis(leafVis),
	m_onionPolygons(onionPolygons), m_onionTree(onionTree), m_onionPortals(onionPortals),
	m_navDatasets(navDatasets), m_objectManager(objectManager), m_modelManager(modelManager)
{
	// Build the collision -> nav poly index lookups.
	int datasetCount = static_cast<int>(m_navDatasets.size());
	for(int i=0; i<datasetCount; ++i)
	{
		m_navDatasets[i]->nav_mesh()->build_collision_to_nav_lookup();
	}
}

//#################### PUBLIC METHODS ####################
BSPTree_CPtr Level::bsp_tree() const
{
	return m_tree;
}

/**
Determine which leaves are potentially visible from the specified eye position.
*/
std::vector<int> Level::find_visible_leaves(const Vector3d& eye) const
{
	bool allVisible = false;
	int curLeaf = TreeUtil::find_leaf_index(eye, m_tree);
	if(curLeaf >= m_tree->empty_leaf_count())
	{
		// If we're erroneously in a solid leaf, we assume all leaves are visible (the best we can do when rendering in this case is render the entire level).
		allVisible = true;
	}

	std::vector<int> visibleLeaves;
	for(int i=0, size=m_leafVis->size(); i<size; ++i)
	{
		// TODO: View frustum culling.
		if(allVisible || (*m_leafVis)(curLeaf,i)) visibleLeaves.push_back(i);
	}

	return visibleLeaves;
}

GeometryRenderer_CPtr Level::geom_renderer() const
{
	return m_geomRenderer;
}

const std::vector<NavDataset_Ptr>& Level::nav_datasets() const
{
	return m_navDatasets;
}

const ObjectManager_Ptr& Level::object_manager()
{
	return m_objectManager;
}

const std::vector<CollisionPolygon_Ptr>& Level::onion_polygons() const
{
	return m_onionPolygons;
}

OnionTree_CPtr Level::onion_tree() const
{
	return m_onionTree;
}

const std::vector<Portal_Ptr>& Level::portals() const
{
	return m_portals;
}

}
