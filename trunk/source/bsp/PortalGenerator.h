/***
 * hesperus: PortalGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_PORTALGENERATOR
#define H_HESP_BSP_PORTALGENERATOR

#include <cmath>
#include <list>

#include <source/math/geom/Polygon.h>
#include "BSPTree.h"
#include "Portal.h"

namespace hesp {

class PortalGenerator
{
	//#################### NESTED CLASSES ####################
public:
	struct PlanePred
	{
		double m_angleTolerance, m_distTolerance;

		PlanePred(double angleTolerance, double distTolerance)
		:	m_angleTolerance(fabs(angleTolerance)), m_distTolerance(fabs(distTolerance))
		{}

		bool operator()(const Plane& lhs, const Plane& rhs) const
		{
			// If these planes are nearly the same (in terms of normal direction
			// and distance value), then !(lhs < rhs) && !(rhs < lhs).
			double angle = acos(lhs.normal().dot(rhs.normal()));
			double dist = lhs.distance_value() - rhs.distance_value();
			if(fabs(angle) < m_angleTolerance && fabs(dist) < m_distTolerance) return false;

			// Otherwise, compare the two planes "lexicographically".
			const Vector3d& nL = lhs.normal(), nR = rhs.normal();
			const double& aL = nL.x, bL = nL.y, cL = nL.z;
			const double& aR = nR.x, bR = nR.y, cR = nR.z;
			const double& dL = lhs.distance_value(), dR = rhs.distance_value();

			return ((aL < aR) ||
					(aL == aR && bL < bR) ||
					(aL == aR && bL == bR && cL < cR) ||
					(aL == aR && bL == bR && cL == cR && dL < dR));
		}
	};

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
	static std::pair<std::list<Portal_Ptr>,bool> clip_portal_to_subtree(const Portal_Ptr& portal, const BSPNode_Ptr& subtreeRoot, PlaneClassifier relativeToPortal = CP_STRADDLE);

	static std::list<Portal_Ptr> clip_portal_to_tree(const Portal_Ptr& portal, const BSPTree_Ptr& tree);

	template <typename Vert, typename AuxData>
	static PlaneList_Ptr find_unique_planes(const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons);

	static Portal_Ptr make_initial_portal(const Plane& plane);
};

}

#include "PortalGenerator.tpp"

#endif
