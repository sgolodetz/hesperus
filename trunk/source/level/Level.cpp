/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Level::Level(const LevelRenderer_Ptr& levelRenderer, const BSPTree_Ptr& tree,
			 const PortalVector& portals, const LeafVisTable_Ptr& leafVis)
:	m_levelRenderer(levelRenderer), m_tree(tree), m_portals(portals), m_leafVis(leafVis)
{}

//#################### PUBLIC METHODS ####################
void Level::render() const
{
	gluLookAt(-1,-10,1, 0,0,0, 0,0,1);
	
	glBegin(GL_LINES);
		glColor3d(1,0,0);
		glVertex3d(0,0,0);
		glVertex3d(10,0,0);

		glColor3d(0,1,0);
		glVertex3d(0,0,0);
		glVertex3d(0,10,0);

		glColor3d(0,0,1);
		glVertex3d(0,0,0);
		glVertex3d(0,0,10);
	glEnd();
}

}
