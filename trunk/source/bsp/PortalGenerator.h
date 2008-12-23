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
private:
	struct PlaneRepPred
	{
		double m_cosAngleTolerance, m_distTolerance;

		PlaneRepPred(double angleTolerance, double distTolerance)
		:	m_cosAngleTolerance(cos(angleTolerance)), m_distTolerance(distTolerance)
		{}

		int operator()(const Plane& lhs, const Plane& rhs) const
		{
			// Note:	This is based closely on a function on p.229 of Graphics Gems III
			//			in the article "Grouping Nearly Coplanar Polygons".
			double cosAngle = lhs.normal().dot(rhs.normal());
			if(cosAngle < -m_cosAngleTolerance) return -1;
			else if(cosAngle > m_cosAngleTolerance) return 1;
			else
			{
				double dist = lhs.distance_value() - rhs.distance_value();
				if(dist < -m_distTolerance) return -1;
				else if(dist > m_distTolerance) return 1;
				else return 0;
			}
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
	static PortalList_Ptr generate_portals(const BSPTree_Ptr& tree, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons);

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
