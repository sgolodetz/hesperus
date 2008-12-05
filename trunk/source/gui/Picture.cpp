/***
 * hesperus: Picture.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Picture.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <gl/gl.h>
#include <gl/glu.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
void Picture::render() const
{
	const int& x1 = m_extents->left(), y1 = m_extents->top(), x2 = m_extents->right(), y2 = m_extents->bottom();
	glViewport(x1, 768 - y2, x2-x1, y2-y1);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(0, x2+1-x1, y2-y1, -1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_POINTS);
		glColor3d(1,0,0);
		glVertex2d(0,0);
		glVertex2d(10,0);
		glVertex2d(10,10);
		glVertex2d(0,10);
	glEnd();
}

}
