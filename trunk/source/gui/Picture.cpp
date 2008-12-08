/***
 * hesperus: Picture.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Picture.h"

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include "Screen.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void Picture::render() const
{
	Screen::instance().set_ortho_viewport(*m_extents);

	glBegin(GL_POINTS);
		glColor3d(1,0,0);
		glVertex2d(0,0);
		glVertex2d(10,0);
		glVertex2d(10,10);
		glVertex2d(0,10);
	glEnd();
}

}
