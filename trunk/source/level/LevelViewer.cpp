/***
 * hesperus: LevelViewer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LevelViewer.h"

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
	glColor3d(1,1,1);
	render_extents();
}

}
