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
#include <source/level/objects/components/ICmpCollision.h>
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

	// Set the skeletons in the object animation controllers.
	std::vector<ObjectID> animatables = m_objectManager->group("Animatables");
	for(size_t i=0, size=animatables.size(); i<size; ++i)
	{
		ICmpRender_Ptr cmpRender = m_objectManager->get_component(animatables[i], cmpRender);
		Skeleton_Ptr skeleton = m_modelManager->model(cmpRender->model_name())->skeleton();
		cmpRender->anim_controller()->set_skeleton(skeleton);
	}

	// Set up the camera.
	m_camera.reset(new FirstPersonCamera(m_objectManager->viewer(), m_objectManager));
}

//#################### PUBLIC METHODS ####################
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
	Vector3d eye = m_camera->set_view();

	// Determine which leaves are potentially visible from the current viewer position.
	bool renderAllLeaves = false;
	int curLeaf = TreeUtil::find_leaf_index(eye, m_tree);
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

	// Render the visible objects.
	render_objects();

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
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);

	std::vector<ObjectID> animatables = m_objectManager->group("Animatables");
	int animatablesCount = static_cast<int>(animatables.size());
	for(int i=0; i<animatablesCount; ++i)
	{
		const ObjectID& animatable = animatables[i];
		if(animatable != m_objectManager->viewer())
		{
			// FIXME: For performance reasons, we should only be rendering objects which are potentially visible.
			ICmpCollision_Ptr cmpCollision = m_objectManager->get_component(animatable, cmpCollision);
			ICmpOrientation_Ptr cmpOrientation = m_objectManager->get_component(animatable, cmpOrientation);
			ICmpPosition_Ptr cmpPosition = m_objectManager->get_component(animatable, cmpPosition);
			ICmpRender_Ptr cmpRender = m_objectManager->get_component(animatable, cmpRender);

			const AABB3d& aabb = m_objectManager->aabbs()[cmpCollision->aabb_indices()[cmpCollision->pose()]];
			const Vector3d& p = cmpPosition->position();
			const Vector3d& n = cmpOrientation->nuv_axes()->n();
			const Vector3d& u = cmpOrientation->nuv_axes()->u();
			const Vector3d& v = cmpOrientation->nuv_axes()->v();

			// Render the model.

			// Project u and n into the horizontal plane (i.e. z = 0) to form uH and nH respectively.
			// Note that the camera is prevented from ever pointing directly upwards/downwards, so
			// renormalizing the resulting vectors isn't a problem.
			Vector3d uH = u;	uH.z = 0;	uH.normalize();
			Vector3d nH = n;	nH.z = 0;	nH.normalize();

			// Note:	This matrix maps x -> uH, -y -> nH, z -> z, and translates by p. Since models are
			//			built in Blender facing in the -y direction, this turns out to be exactly the
			//			transformation required to render the models with the correct position and
			//			orientation.
			RBTMatrix_Ptr mat = RBTMatrix::zeros();
			RBTMatrix& m = *mat;
			m(0,0) = uH.x;		m(0,1) = -nH.x;		/*m(0,2) = 0;*/		m(0,3) = p.x;
			m(1,0) = uH.y;		m(1,1) = -nH.y;		/*m(1,2) = 0;*/		m(1,3) = p.y;
			/*m(2,0) = 0;*/		/*m(2,1) = 0;*/		m(2,2) = 1;			m(2,3) = p.z;

			glPushMatrix();
			glMultMatrixd(&m.rep()[0]);

			Model_Ptr model = m_modelManager->model(cmpRender->model_name());
			model->render(cmpRender->anim_controller());

			glPopMatrix();

			// Render the AABB.
			AABB3d tAABB = aabb.translate(p);
			const Vector3d& mins = tAABB.minimum();
			const Vector3d& maxs = tAABB.maximum();

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_CULL_FACE);
			glColor3d(1,1,0);

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

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);

			// Render the entity camera axes.
			Vector3d pn = p + n;
			Vector3d pu = p + u;
			Vector3d pv = p + v;
			glBegin(GL_LINES);
				glColor3d(1,0,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pn.x, pn.y, pn.z);
				glColor3d(0,1,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pu.x, pu.y, pu.z);
				glColor3d(0,0,1);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pv.x, pv.y, pv.z);
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
