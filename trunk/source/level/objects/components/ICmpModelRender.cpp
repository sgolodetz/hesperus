/***
 * hesperus: ICmpModelRender.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ICmpModelRender.h"

namespace hesp {

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
