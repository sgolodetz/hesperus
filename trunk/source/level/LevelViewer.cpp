/***
 * hesperus: LevelViewer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LevelViewer.h"

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/gui/Screen.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
LevelViewer::LevelViewer(const Level_Ptr& level)
:	m_level(level)
{}

//#################### PUBLIC METHODS ####################
void LevelViewer::render() const
{
	// Draw the level itself.
	Screen::instance().set_persp_viewport(*m_extents, 45.0, 0.1, 4096.0);
	m_level->render();

	// Draw a white border round the component on the screen.
	Screen::instance().set_ortho_viewport(*m_extents);
	const int& x1 = m_extents->left(), y1 = m_extents->top(), x2 = m_extents->right(), y2 = m_extents->bottom();
	const int w = x2 - x1, h = y2 - y1;
	glBegin(GL_LINE_LOOP);
		glColor3d(1,1,1);
		glVertex2i(0,0);
		glVertex2i(w,0);
		glVertex2i(w,h);
		glVertex2i(0,h);
	glEnd();
}

}
