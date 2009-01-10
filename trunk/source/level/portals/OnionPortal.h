/***
 * hesperus: OnionPortal.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_PORTALS_ONIONPORTAL
#define H_HESP_LEVEL_PORTALS_ONIONPORTAL

#include <set>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/Polygon.h>

namespace hesp {

//#################### CLASSES ####################
struct OnionPortalInfo
{
	int fromLeaf;
	int toLeaf;
	std::set<int> fromMapIndices;	// the maps in which this portal's from leaf appears
	std::set<int> toMapIndices;		// the maps in which this portal's to leaf appears
	std::set<int> mapIndices;		// the maps in which this portal appears
};

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d,OnionPortalInfo> OnionPortal;
typedef shared_ptr<OnionPortal> OnionPortal_Ptr;

}

#endif
