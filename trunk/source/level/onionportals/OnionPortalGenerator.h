/***
 * hesperus: OnionPortalGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_ONIONPORTALS_ONIONPORTALGENERATOR
#define H_HESP_LEVEL_ONIONPORTALS_ONIONPORTALGENERATOR

#include <list>

#include <source/level/onionbsp/OnionTree.h>
#include <source/math/geom/Polygon.h>
#include "OnionPortal.h"

namespace hesp {

class OnionPortalGenerator
{
	//#################### TYPEDEFS ####################
private:
	typedef std::list<Plane> PlaneList;
	typedef shared_ptr<PlaneList> PlaneList_Ptr;
	typedef std::list<OnionPortal_Ptr> OnionPortalList;
	typedef shared_ptr<OnionPortalList> OnionPortalList_Ptr;

	//#################### PUBLIC METHODS ####################
public:
	template <typename Poly>
	static OnionPortalList_Ptr generate_portals(const std::vector<shared_ptr<Poly> >& polygons, const OnionTree_Ptr& tree);

	//#################### PRIVATE METHODS ####################
private:
	static OnionPortalList clip_portal_to_subtree(const OnionPortal_Ptr& portal, const OnionNode_Ptr& subtreeRoot,
												  PlaneClassifier relativeToPortal = CP_STRADDLE);

	static OnionPortalList clip_portal_to_tree(const OnionPortal_Ptr& portal, const OnionTree_Ptr& tree);

	static OnionPortal_Ptr make_initial_portal(const Plane& plane);
};

}

#include "OnionPortalGenerator.tpp"

#endif
