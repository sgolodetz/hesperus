/***
 * hesperus: BasePortalGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_PORTALS_BASEPORTALGENERATOR
#define H_HESP_LEVEL_PORTALS_BASEPORTALGENERATOR

#include <list>
#include <vector>

#include <source/math/geom/Plane.h>

namespace hesp {

template <typename PortalT, typename TreeT, typename NodeT>
class BasePortalGenerator
{
	//#################### TYPEDEFS ####################
private:
	typedef std::list<Plane> PlaneList;
	typedef shared_ptr<PlaneList> PlaneList_Ptr;
	typedef shared_ptr<PortalT> PortalT_Ptr;
	typedef std::list<PortalT_Ptr> PortalTList;
	typedef shared_ptr<PortalTList> PortalTList_Ptr;
	typedef shared_ptr<TreeT> TreeT_Ptr;
	typedef shared_ptr<NodeT> NodeT_Ptr;

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual PortalTList clip_portal_to_subtree(const PortalT_Ptr& portal, const NodeT_Ptr& subtreeRoot,
											   PlaneClassifier relativeToPortal = CP_STRADDLE) const = 0;

	//#################### DESTRUCTOR ####################
public:
	virtual ~BasePortalGenerator() {}

	//#################### PUBLIC METHODS ####################
public:
	template <typename Poly>
	PortalTList_Ptr generate_portals(const std::vector<shared_ptr<Poly> >& polygons, const TreeT_Ptr& tree) const;

	//#################### PRIVATE METHODS ####################
private:
	PortalTList clip_portal_to_tree(const PortalT_Ptr& portal, const TreeT_Ptr& tree) const;

	PortalT_Ptr make_initial_portal(const Plane& plane) const;
};

}

#include "BasePortalGenerator.tpp"

#endif
