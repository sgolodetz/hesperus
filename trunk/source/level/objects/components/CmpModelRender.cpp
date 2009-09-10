/***
 * hesperus: CmpModelRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpModelRender.h"

#include <source/ogl/WrappedGL.h>

#include <source/level/models/AnimationController.h>
#include <source/level/models/Model.h>
#include <source/level/models/ModelManager.h>
#include "ICmpAABBBounds.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpModelRender::CmpModelRender(const std::string& modelName)
:	m_modelName(modelName), m_animController(new AnimationController), m_highlights(false)
{}

//#################### PUBLIC METHODS ####################
void CmpModelRender::set_highlights(bool enabled)
{
	m_highlights = enabled;
}

void CmpModelRender::set_model_manager(const ModelManager_Ptr& modelManager)
{
	m_modelManager = modelManager;
	m_modelManager->register_model(m_modelName);
}

void CmpModelRender::set_skeleton()
{
	m_animController->set_skeleton(skeleton());
}

//#################### PROTECTED METHODS ####################
const Model_Ptr& CmpModelRender::model()	{ return m_modelManager->model(m_modelName); }
Model_CPtr CmpModelRender::model() const	{ return m_modelManager->model(m_modelName); }

void CmpModelRender::render_aabb(const Vector3d& p) const
{
	ICmpAABBBounds_Ptr cmpBounds = m_objectManager->get_component(m_objectID, cmpBounds);
	if(!cmpBounds) return;

	const AABB3d& aabb = m_objectManager->aabbs()[cmpBounds->cur_aabb_index()];
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
}

void CmpModelRender::render_nuv_axes(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v)
{
	Vector3d pn = p + n;
	Vector3d pu = p + u;
	Vector3d pv = p + v;
	glBegin(GL_LINES);
		glColor3d(1,0,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pn.x, pn.y, pn.z);
		glColor3d(0,1,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pu.x, pu.y, pu.z);
		glColor3d(0,0,1);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pv.x, pv.y, pv.z);
	glEnd();
}

const Skeleton_Ptr& CmpModelRender::skeleton()	{ return m_modelManager->model(m_modelName)->skeleton(); }
Skeleton_CPtr CmpModelRender::skeleton() const	{ return m_modelManager->model(m_modelName)->skeleton(); }

}
