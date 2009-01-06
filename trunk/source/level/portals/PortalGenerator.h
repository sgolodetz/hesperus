/***
 * hesperus: PortalGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_PORTALS_PORTALGENERATOR
#define H_HESP_LEVEL_PORTALS_PORTALGENERATOR

#include <cmath>
#include <list>

#include <source/level/bsp/BSPTree.h>
#include <source/math/geom/Polygon.h>
#include "Portal.h"

namespace hesp {

class PortalGenerator
{
	//#################### TYPEDEFS ####################
private:
	typedef std::list<Plane> PlaneList;
	typedef shared_ptr<PlaneList> PlaneList_Ptr;
	typedef std::list<Portal_Ptr> PortalList;
	typedef shared_ptr<PortalList> PortalList_Ptr;

	//#################### PUBLIC METHODS ####################
public:
	template <typename Vert, typename AuxData>
	static PortalList_Ptr generate_portals(const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, const BSPTree_Ptr& tree);

	//#################### PRIVATE METHODS ####################
private:
	static PortalList clip_portal_to_subtree(const Portal_Ptr& portal, const BSPNode_Ptr& subtreeRoot, PlaneClassifier relativeToPortal = CP_STRADDLE);

	static PortalList clip_portal_to_tree(const Portal_Ptr& portal, const BSPTree_Ptr& tree);

	template <typename Vert, typename AuxData>
	static PlaneList_Ptr find_unique_planes(const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons);

	static Portal_Ptr make_initial_portal(const Plane& plane);
};

}

#include "PortalGenerator.tpp"

#endif
