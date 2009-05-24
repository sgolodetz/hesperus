/***
 * hesperus: CmpModelRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpModelRender.h"

#include <source/level/objects/base/ObjectManager.h>
#include "ICmpAABBBounds.h"
#include "ICmpOrientation.h"
#include "ICmpPosition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpModelRender::CmpModelRender(const std::string& modelName)
:	m_modelName(modelName), m_animController(new AnimationController)
{}

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpModelRender::load(const Properties& properties)
{
	return IComponent_Ptr(new CmpModelRender(properties.get_actual<std::string>("ModelName")));
}

//#################### PUBLIC METHODS ####################
AnimationController_Ptr CmpModelRender::anim_controller() const
{
	return m_animController;
}

void CmpModelRender::check_dependencies() const
{
	check_dependency<ICmpOrientation>();
	check_dependency<ICmpPosition>();
}

void CmpModelRender::render() const
{
	ICmpOrientation_Ptr cmpOrientation = m_objectManager->get_component(m_objectID, cmpOrientation);
	ICmpPosition_Ptr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);

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

	Model_Ptr model = m_modelManager->model(m_modelName);
	model->render(m_animController);

	glPopMatrix();

	// Render the object's AABB (if any).
	ICmpAABBBounds_Ptr cmpBounds = m_objectManager->get_component(m_objectID, cmpBounds);
	if(cmpBounds)
	{
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

	// Render the object's NUV axes.
	Vector3d pn = p + n;
	Vector3d pu = p + u;
	Vector3d pv = p + v;
	glBegin(GL_LINES);
		glColor3d(1,0,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pn.x, pn.y, pn.z);
		glColor3d(0,1,0);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pu.x, pu.y, pu.z);
		glColor3d(0,0,1);	glVertex3d(p.x, p.y, p.z);	glVertex3d(pv.x, pv.y, pv.z);
	glEnd();
}

std::pair<std::string,Properties> CmpModelRender::save() const
{
	Properties properties;
	properties.set_actual("ModelName", m_modelName);
	return std::make_pair("ModelRender", properties);
}

void CmpModelRender::set_model_manager(const ModelManager_Ptr& modelManager)
{
	m_modelManager = modelManager;
	m_modelManager->register_model(m_modelName);
}

void CmpModelRender::set_skeleton()
{
	Skeleton_Ptr skeleton = m_modelManager->model(m_modelName)->skeleton();
	m_animController->set_skeleton(skeleton);
}

}
