/***
 * hesperus: PortalGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_PORTALGENERATOR
#define H_HESP_BSP_PORTALGENERATOR

#include <list>

#include <source/math/geom/Polygon.h>
#include "BSPTree.h"
#include "Portal.h"

namespace hesp {

class PortalGenerator
{
	//#################### NESTED CLASSES ####################
private:
	struct PlaneRepPred
	{
		double m_angleTolerance, m_distTolerance;

		PlaneRepPred(double angleTolerance, double distTolerance)
		:	m_angleTolerance(angleTolerance), m_distTolerance(distTolerance)
		{}

		int operator()(const Plane& lhs, const Plane& rhs) const
		{
			// NYI
			throw 23;
		}
	};

	//#################### TYPEDEFS ####################
private:
	typedef std::list<Plane> PlaneList;
	typedef shared_ptr<PlaneList> PlaneList_Ptr;
	typedef std::vector<Portal_Ptr> PortalVector;
	typedef shared_ptr<PortalVector> PortalVector_Ptr;

	//#################### PUBLIC METHODS ####################
public:
	template <typename Vert, typename AuxData>
	static PortalVector_Ptr generate_portals(const BSPTree_Ptr& tree, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons);

	//#################### PRIVATE METHODS ####################
private:
	static std::pair<std::list<Portal_Ptr>,bool> clip_portal_to_subtree(const Portal_Ptr& portal, const BSPNode_Ptr& subtreeRoot, PlaneClassifier relativeToPortal = CP_STRADDLE);

	template <typename Vert, typename AuxData>
	static PlaneList_Ptr find_unique_planes(const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons);

	static Portal_Ptr make_initial_portal(const Plane& plane);
};

}

#include "PortalGenerator.tpp"

#endif
