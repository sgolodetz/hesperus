/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

#include <algorithm>
#include <iostream>

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
const EntityManager_Ptr& Level::entity_manager() const
{
	return m_entityManager;
}

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

	Entity_Ptr player = m_entityManager->player();
	const Vector3d& pos = player->camera_component()->camera().position();
	const Vector3d& look = player->camera_component()->camera().n();

	// Calculate the player's eye position and where they're looking at.
	const AABB3d& aabb = m_entityManager->aabb(player->collision_component()->pose());
	Vector3d eye = pos + Vector3d(0,0,aabb.maximum().z * 0.9);
	Vector3d at = eye + look;

	// Set the camera accordingly.
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, 0, 0, 1);

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
#if 0
	std::cout << "Polygon Count " << polyIndices.size() << std::endl;
#endif

	// Render the visible entities.
	render_entities();

	glPopAttrib();
}

//#################### PRIVATE METHODS ####################
void Level::render_entities() const
{
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glColor3d(1,1,0);

	const std::vector<Entity_Ptr>& visibles = m_entityManager->visibles();
	int visiblesCount = static_cast<int>(visibles.size());
	for(int i=0; i<visiblesCount; ++i)
	{
		// FIXME: Eventually we'll just call render(m_tree, m_leafVis) on each visible entity.
		ICameraComponent_Ptr camComponent = visibles[i]->camera_component();
		ICollisionComponent_Ptr colComponent = visibles[i]->collision_component();
#if 0
		IPositionComponent_Ptr posComponent = visibles[i]->position_component();
#endif
		if(camComponent && colComponent && visibles[i]->entity_class() != "Player")
		{
			const AABB3d& aabb = m_entityManager->aabb(colComponent->aabb_indices()[colComponent->pose()]);
			AABB3d tAABB = aabb.translate(camComponent->camera().position());
			const Vector3d& mins = tAABB.minimum();
			const Vector3d& maxs = tAABB.maximum();

			// Render the translated AABB.
			glBegin(GL_QUADS);
				// Front
				glVertex3d(mins.x, mins.y, mins.z);
				glVertex3d(maxs.x, mins.y, mins.z);
				glVertex3d(maxs.x, mins.y, maxs.z);
				glVertex3d(mins.x, mins.y, maxs.z);

				// Right
				glVertex3d(maxs.x, mins.y, mins.z);
				glVertex3d(maxs.x, maxs.y, mins.z);
				glVertex3d(maxs.x, maxs.y, maxs.z);
				glVertex3d(maxs.x, mins.y, maxs.z);

				// Back
				glVertex3d(maxs.x, maxs.y, mins.z);
				glVertex3d(mins.x, maxs.y, mins.z);
				glVertex3d(mins.x, maxs.y, maxs.z);
				glVertex3d(maxs.x, maxs.y, maxs.z);

				// Left
				glVertex3d(mins.x, maxs.y, mins.z);
				glVertex3d(mins.x, mins.y, mins.z);
				glVertex3d(mins.x, mins.y, maxs.z);
				glVertex3d(mins.x, maxs.y, maxs.z);
			glEnd();
		}
	}

	glPopAttrib();
}

}
