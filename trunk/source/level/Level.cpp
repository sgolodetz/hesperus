/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

#include <algorithm>

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#include <source/math/vectors/Vector3.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Level::Level(const LevelRenderer_Ptr& levelRenderer, const BSPTree_Ptr& tree,
			 const PortalVector& portals, const LeafVisTable_Ptr& leafVis)
:	m_levelRenderer(levelRenderer), m_tree(tree), m_portals(portals), m_leafVis(leafVis)
{}

//#################### PUBLIC METHODS ####################
void Level::render() const
{
#if 0
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
#endif

	const bool RENDER_ALL_LEAVES = true;

	const Vector3d pos(5, 15, 2);
	const Vector3d look(1,0,0);
	Vector3d at = pos + look;

	gluLookAt(pos.x, pos.y, pos.z, at.x, at.y, at.z, 0, 0, 1);

	int curLeaf = m_tree->find_leaf_index(pos);
	std::vector<int> visibleLeaves;
	for(int i=0, size=m_leafVis->size(); i<size; ++i)
	{
		if(RENDER_ALL_LEAVES || (*m_leafVis)(curLeaf,i)) visibleLeaves.push_back(i);
	}

	std::vector<int> polyIndices;
	for(std::vector<int>::const_iterator it=visibleLeaves.begin(), iend=visibleLeaves.end(); it!=iend; ++it)
	{
		const BSPLeaf *leaf = m_tree->leaf(*it);
		std::copy(leaf->polygon_indices().begin(), leaf->polygon_indices().end(), std::back_inserter(polyIndices));
	}
	m_levelRenderer->render(polyIndices);
}

}
