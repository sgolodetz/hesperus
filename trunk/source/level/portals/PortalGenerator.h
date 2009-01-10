/***
 * hesperus: PortalGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_PORTALS_PORTALGENERATOR
#define H_HESP_LEVEL_PORTALS_PORTALGENERATOR

#include <source/level/bsp/BSPBranch.h>
#include <source/level/bsp/BSPTree.h>
#include "BasePortalGenerator.h"
#include "Portal.h"

namespace hesp {

class PortalGenerator : public BasePortalGenerator<Portal,BSPTree,BSPNode,BSPBranch,BSPLeaf>
{
	//#################### TYPEDEFS ####################
private:
	typedef std::list<Portal_Ptr> PortalList;
	typedef shared_ptr<PortalList> PortalList_Ptr;

	//#################### PRIVATE METHODS ####################
private:
	PortalList clip_portal_to_leaf(const Portal_Ptr& portal, const BSPLeaf *leaf, PlaneClassifier relativeToPortal) const;
};

}

#endif
