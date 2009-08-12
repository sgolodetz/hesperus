/***
 * hesperus: ICmpModelRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ICmpModelRender.h"

#include "ICmpAABBBounds.h"

namespace hesp {

void ICmpModelRender::render_aabb(const Vector3d& p) const
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

void ICmpModelRender::render_nuv_axes(const Vector3d& p, const Vector3d& n, const Vector3d& u, const Vector3d& v)
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

}
