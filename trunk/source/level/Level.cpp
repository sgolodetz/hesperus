/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

#include <algorithm>
#include <iostream>

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/cameras/FirstPersonCamera.h>
#include <source/colours/Colour3d.h>
#include <source/level/models/Model.h>
#include <source/level/objects/components/ICmpAABBBounds.h>
#include <source/level/objects/components/ICmpCharacterModelRender.h>
#include <source/level/objects/components/ICmpOrientation.h>
#include <source/level/objects/components/ICmpPosition.h>
#include <source/level/objects/components/ICmpRender.h>
#include <source/level/trees/TreeUtil.h>
#include <source/math/vectors/Vector3.h>

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

	// Set up the camera.
	m_camera.reset(new FirstPersonCamera(m_objectManager->player(), m_objectManager));
}

//#################### PUBLIC METHODS ####################
const Camera_Ptr& Level::camera() const
{
	return m_camera;
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

const std::vector<NavDataset_Ptr>& Level::nav_datasets() const
{
	return m_navDatasets;
}

const ObjectManager_Ptr& Level::object_manager() const
{
	return m_objectManager;
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

	// Set the view.
	m_camera->update();
	Vector3d eye = m_camera->eye(), at = m_camera->at(), up = m_camera->up();
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);

	// Determine which leaves are potentially visible from the current viewer position.
	std::vector<int> visibleLeaves = find_visible_leaves(eye);

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

	// Render the navigation meshes.
#if 0
	render_navmeshes();
	render_navlinks();
#endif

	// Render the portals.
#if 0
	render_portals();
#endif

	// Render the visible objects. (These must be done after everything else to ensure that
	// things like the crosshair and active item are not obscured by the rest of the scene
	// when rendering in first-person.)
	render_objects();

	glPopAttrib();
}

//#################### PRIVATE METHODS ####################
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

void Level::render_objects() const
{
	// FIXME: For performance reasons, we should only be rendering objects which are potentially visible.

	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	std::vector<ObjectID> renderables = m_objectManager->group("Renderables");
	ICmpCharacterModelRender_Ptr cmpFirstPersonRender;
	for(size_t i=0, size=renderables.size(); i<size; ++i)
	{
		const ObjectID& renderable = renderables[i];
		if(renderable != m_objectManager->player() || !m_camera->is_inside_player())
		{
			ICmpRender_Ptr cmpRender = m_objectManager->get_component(renderable, cmpRender);
			cmpRender->render();
		}
		else
		{
			cmpFirstPersonRender = m_objectManager->get_component(renderable, cmpFirstPersonRender);
			if(!cmpFirstPersonRender) throw Exception("For the player to be renderable in first-person, it must have a CharacterModelRender component");
		}
	}

	// Note:	First-person rendering means rendering the active item (e.g. the weapon held by the player) but
	//			not the actual player model.
	if(cmpFirstPersonRender) cmpFirstPersonRender->render_first_person();

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
