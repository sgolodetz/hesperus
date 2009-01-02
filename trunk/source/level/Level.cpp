/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Level::Level(const LevelRenderer_Ptr& levelRenderer, const BSPTree_Ptr& tree,
			 const PortalVector& portals, const LeafVisTable_Ptr& leafVis)
:	m_levelRenderer(levelRenderer), m_tree(tree), m_portals(portals), m_leafVis(leafVis)
{}

//#################### PUBLIC METHODS ####################
void Level::render() const
{
	// NYI
	throw 23;
}

}
