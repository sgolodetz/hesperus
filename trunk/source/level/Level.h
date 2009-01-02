/***
 * hesperus: Level.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LEVEL
#define H_HESP_LEVEL_LEVEL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/bsp/BSPTree.h>
#include <source/level/portals/Portal.h>
#include <source/level/vis/VisTable.h>
#include "LevelRenderer.h"

namespace hesp {

class Level
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<Portal_Ptr> PortalVector;

	//#################### PRIVATE VARIABLES ####################
private:
	LevelRenderer_Ptr m_levelRenderer;
	BSPTree_Ptr m_tree;
	PortalVector m_portals;
	LeafVisTable_Ptr m_leafVis;

	//#################### CONSTRUCTORS ####################
public:
	Level(const LevelRenderer_Ptr& levelRenderer, const BSPTree_Ptr& tree,
		  const PortalVector& portals, const LeafVisTable_Ptr& leafVis);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Level> Level_Ptr;
typedef shared_ptr<const Level> Level_CPtr;

}

#endif
