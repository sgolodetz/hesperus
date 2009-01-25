/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

#include <algorithm>

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/math/vectors/Vector3.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Level::Level(const GeometryRenderer_Ptr& geomRenderer, const BSPTree_Ptr& tree,
			 const PortalVector& portals, const LeafVisTable_Ptr& leafVis,
			 const ColPolyVector& onionPolygons, const OnionTree_Ptr& onionTree,
			 const OnionPortalVector& onionPortals, const std::vector<NavDataset_Ptr>& navDatasets,
			 const EntityManager_Ptr& entityManager)
:	m_geomRenderer(geomRenderer), m_tree(tree), m_portals(portals), m_leafVis(leafVis),
	m_onionPolygons(onionPolygons), m_onionTree(onionTree), m_onionPortals(onionPortals),
	m_entityManager(entityManager)
{
	// TODO: Navigation stuff
}

//#################### PUBLIC METHODS ####################
void Level::render() const
{
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	// Enable back-face culling.
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	// Enable the z-buffer.
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	// TEMPORARY: The player position and look vector will eventually be obtained from elsewhere.
	const Vector3d pos(6, 24, 7);
	const Vector3d look(1,-0.5,0);
	
	// Set the camera.
	Vector3d at = pos + look;
	gluLookAt(pos.x, pos.y, pos.z, at.x, at.y, at.z, 0, 0, 1);

	// Determine which leaves are potentially visible from the current player position.
	bool renderAllLeaves = false;
	int curLeaf = m_tree->find_leaf_index(pos);
	if(curLeaf >= m_tree->empty_leaf_count())
	{
		// If we're erroneously in a solid leaf, the best we can do is render the entire level.
		renderAllLeaves = true;
	}

	std::vector<int> visibleLeaves;
	for(int i=0, size=m_leafVis->size(); i<size; ++i)
	{
		// TODO: View frustum culling.
		if(renderAllLeaves || (*m_leafVis)(curLeaf,i)) visibleLeaves.push_back(i);
	}

	// Make a list of all the polygons which need rendering and pass them to the renderer.
	std::vector<int> polyIndices;
	for(std::vector<int>::const_iterator it=visibleLeaves.begin(), iend=visibleLeaves.end(); it!=iend; ++it)
	{
		const BSPLeaf *leaf = m_tree->leaf(*it);
		std::copy(leaf->polygon_indices().begin(), leaf->polygon_indices().end(), std::back_inserter(polyIndices));
	}
	m_geomRenderer->render(polyIndices);

	glPopAttrib();
}

}
