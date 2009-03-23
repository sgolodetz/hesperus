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
#include <source/level/models/Model.h>
#include <source/math/vectors/Vector3.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Level::Level(const GeometryRenderer_Ptr& geomRenderer, const BSPTree_Ptr& tree,
			 const PortalVector& portals, const LeafVisTable_Ptr& leafVis,
			 const ColPolyVector& onionPolygons, const OnionTree_Ptr& onionTree,
			 const OnionPortalVector& onionPortals, const NavDatasetVector& navDatasets,
			 const EntityManager_Ptr& entityManager, const ModelManager_Ptr& modelManager)
:	m_geomRenderer(geomRenderer), m_tree(tree), m_portals(portals), m_leafVis(leafVis),
	m_onionPolygons(onionPolygons), m_onionTree(onionTree), m_onionPortals(onionPortals),
	m_navDatasets(navDatasets), m_entityManager(entityManager), m_modelManager(modelManager)
{
	// Build the collision -> nav poly index lookups.
	int datasetCount = static_cast<int>(m_navDatasets.size());
	for(int i=0; i<datasetCount; ++i)
	{
		m_navDatasets[i]->nav_mesh()->build_collision_to_nav_lookup();
	}

	// Set the skeletons in the entity animation controllers.
	const std::vector<Entity_Ptr>& animatables = m_entityManager->animatables();
	int animatableCount = static_cast<int>(animatables.size());
	for(int i=0; i<animatableCount; ++i)
	{
		IAnimationComponent_Ptr animComponent = animatables[i]->animation_component();
		Skeleton_Ptr skeleton = m_modelManager->model(animComponent->model_name())->skeleton();
		animComponent->anim_controller()->set_skeleton(skeleton);
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

	const std::vector<Entity_Ptr>& animatables = m_entityManager->animatables();
	int animatablesCount = static_cast<int>(animatables.size());
	for(int i=0; i<animatablesCount; ++i)
	{
		IAnimationComponent_Ptr animComponent = animatables[i]->animation_component();
		ICameraComponent_Ptr camComponent = animatables[i]->camera_component();
		ICollisionComponent_Ptr colComponent = animatables[i]->collision_component();
		if(animComponent && camComponent && colComponent && animatables[i] != m_entityManager->viewer())
		{
			// FIXME: For performance reasons, we should only be rendering entities which are potentially visible.
			const Camera& camera = camComponent->camera();
			const AABB3d& aabb = m_entityManager->aabbs()[colComponent->aabb_indices()[colComponent->pose()]];
			const Vector3d& p = camera.position();
			const Vector3d& n = camera.n();
			const Vector3d& u = camera.u();
			const Vector3d& v = camera.v();

			// Render the model.
			AnimationController_Ptr animController = animComponent->anim_controller();

#if 1
			// FIXME: Animations should eventually be requested from elsewhere.
			static int blah = 0;
			++blah;
			int blah2 = blah/250;
			if(blah2 % 2 == 0)
			{
				animController->request_animation("walk");
			}
			else
			{
				animController->request_animation("<rest>");
			}
#endif

			// Note:	This matrix maps x -> u, -y -> n, z -> z, and translates by p. Since models are
			//			built in Blender facing in the -y direction, this turns out to be exactly the
			//			transformation required to render the models with the correct position and
			//			orientation.
			RBTMatrix_Ptr mat = RBTMatrix::zeros();
			RBTMatrix& m = *mat;
			m(0,0) = u.x;		m(0,1) = -n.x;		/*m(0,2) = 0;*/		m(0,3) = p.x;
			m(1,0) = u.y;		m(1,1) = -n.y;		/*m(1,2) = 0;*/		m(1,3) = p.y;
			m(2,0) = u.z;		m(2,1) = -n.z;		m(2,2) = 1;			m(2,3) = p.z;

			glPushMatrix();
			glMultMatrixd(&m.rep()[0]);

			Model_Ptr model = m_modelManager->model(animComponent->model_name());
			model->render(animController);

			glPopMatrix();

			// Render the AABB.
			AABB3d tAABB = aabb.translate(camera.position());
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
