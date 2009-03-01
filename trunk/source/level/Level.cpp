/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

#include <algorithm>
#include <iostream>

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/colours/Colour3d.h>
#include <source/math/vectors/Vector3.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Level::Level(const GeometryRenderer_Ptr& geomRenderer, const BSPTree_Ptr& tree,
			 const PortalVector& portals, const LeafVisTable_Ptr& leafVis,
			 const ColPolyVector& onionPolygons, const OnionTree_Ptr& onionTree,
			 const OnionPortalVector& onionPortals, const NavDatasetVector& navDatasets,
			 const EntityManager_Ptr& entityManager)
:	m_geomRenderer(geomRenderer), m_tree(tree), m_portals(portals), m_leafVis(leafVis),
	m_onionPolygons(onionPolygons), m_onionTree(onionTree), m_onionPortals(onionPortals),
	m_navDatasets(navDatasets), m_entityManager(entityManager)
{
	// Build the collision -> nav poly index lookups.
	int datasetCount = static_cast<int>(m_navDatasets.size());
	for(int i=0; i<datasetCount; ++i)
	{
		m_navDatasets[i]->nav_mesh()->build_collision_to_nav_lookup();
	}
}

//#################### PUBLIC METHODS ####################
const EntityManager_Ptr& Level::entity_manager() const
{
	return m_entityManager;
}

const std::vector<NavDataset_Ptr>& Level::nav_datasets() const
{
	return m_navDatasets;
}

const std::vector<CollisionPolygon_Ptr>& Level::onion_polygons() const
{
	return m_onionPolygons;
}

const OnionTree_Ptr& Level::onion_tree() const
{
	return m_onionTree;
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

	Entity_Ptr viewer = m_entityManager->viewer();
	const Vector3d& pos = viewer->camera_component()->camera().position();
	const Vector3d& look = viewer->camera_component()->camera().n();

	// Calculate the viewer's eye position and where they're looking at.
	const AABB3d& aabb = m_entityManager->aabbs()[viewer->collision_component()->pose()];
	Vector3d eye = pos + Vector3d(0,0,aabb.maximum().z * 0.9);
	Vector3d at = eye + look;

	// Set the camera accordingly.
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, 0, 0, 1);

	// Determine which leaves are potentially visible from the current viewer position.
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

	// Render the navigation meshes.
#if 1
	render_navmeshes();
	render_navlinks();
#endif

	// Render the portals.
#if 0
	render_portals();
#endif

	glPopAttrib();
}

//#################### PRIVATE METHODS ####################
void Level::render_entities() const
{
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glColor3d(1,1,0);

	const std::vector<Entity_Ptr>& animatables = m_entityManager->animatables();
	int animatablesCount = static_cast<int>(animatables.size());
	for(int i=0; i<animatablesCount; ++i)
	{
		// FIXME: Eventually we'll just call render(m_tree, m_leafVis) on each visible entity.
		ICameraComponent_Ptr camComponent = animatables[i]->camera_component();
		ICollisionComponent_Ptr colComponent = animatables[i]->collision_component();
		if(camComponent && colComponent && animatables[i] != m_entityManager->viewer())
		{
			const Camera& camera = camComponent->camera();
			const AABB3d& aabb = m_entityManager->aabbs()[colComponent->aabb_indices()[colComponent->pose()]];
			AABB3d tAABB = aabb.translate(camera.position());
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

			// Render the entity camera axes.
			const Vector3d& p = camera.position();
			Vector3d pn = p + camera.n();
			Vector3d pu = p + camera.u();
			Vector3d pv = p + camera.v();
			glBegin(GL_LINES);
				glColor3d(1,0,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pn.x, pn.y, pn.z);
				glColor3d(0,1,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pu.x, pu.y, pu.z);
				glColor3d(0,0,1);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pv.x, pv.y, pv.z);
			glEnd();
		}
	}

	glPopAttrib();
}

void Level::render_navlinks() const
{
	int datasetCount = static_cast<int>(m_navDatasets.size());
	for(int i=0; i<datasetCount; ++i)
	{
		const std::vector<NavLink_Ptr>& navLinks = m_navDatasets[i]->nav_mesh()->links();
		int linkCount = static_cast<int>(navLinks.size());
		for(int j=0; j<linkCount; ++j)
		{
			navLinks[j]->render();
		}
	}
}

void Level::render_navmeshes() const
{
	// Note: Navmeshes don't need to be rendered efficiently, since we only render them for testing purposes anyway.

	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);

	Colour3d colours[] = { Colour3d(1,0,0), Colour3d(0,1,0), Colour3d(0,0,1) };
	int colourCount = sizeof(colours)/sizeof(Colour3d);

	int datasetCount = static_cast<int>(m_navDatasets.size());
	for(int i=0; i<datasetCount; ++i)
	{
		int c = i % colourCount;
		glColor3d(colours[c].r, colours[c].g, colours[c].b);

		const std::vector<NavPolygon_Ptr>& navPolys = m_navDatasets[i]->nav_mesh()->polygons();
		int polyCount = static_cast<int>(navPolys.size());
		for(int j=0; j<polyCount; ++j)
		{
			const CollisionPolygon_Ptr& onionPoly = m_onionPolygons[navPolys[j]->collision_poly_index()];
			int vertCount = onionPoly->vertex_count();
			glBegin(GL_POLYGON);
				for(int k=0; k<vertCount; ++k)
				{
					const Vector3d& v = onionPoly->vertex(k);
					glVertex3d(v.x, v.y, v.z);
				}
			glEnd();
		}
	}

	glPopAttrib();
}

void Level::render_portals() const
{
	// Note: Portals don't need to be rendered efficiently, since we only render them for testing purposes anyway.

	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);

	glColor3d(1,1,1);

	for(PortalVector::const_iterator it=m_portals.begin(), iend=m_portals.end(); it!=iend; ++it)
	{
		const Portal& portal = **it;
		int vertCount = portal.vertex_count();
		glBegin(GL_POLYGON);
			for(int j=0; j<vertCount; ++j)
			{
				const Vector3d& v = portal.vertex(j);
				glVertex3d(v.x, v.y, v.z);
			}
		glEnd();
	}

	glPopAttrib();
}

}
