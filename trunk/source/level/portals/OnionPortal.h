/***
 * hesperus: OnionPortal.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_PORTALS_ONIONPORTAL
#define H_HESP_LEVEL_PORTALS_ONIONPORTAL

#include <iosfwd>
#include <set>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/Polygon.h>

namespace hesp {

//#################### CLASSES ####################
struct OnionPortalInfo
{
	//#################### PUBLIC VARIABLES ####################
	int fromLeaf;
	int toLeaf;
	std::set<int> fromMapIndices;	// the maps in which this portal's from leaf appears (only used during portal generation)
	std::set<int> toMapIndices;		// the maps in which this portal's to leaf appears (only used during portal generation)
	std::set<int> mapIndices;		// the maps in which this portal appears

	//#################### PUBLIC METHODS ####################
	OnionPortalInfo flip() const
	{
		OnionPortalInfo ret;
		ret.fromLeaf = toLeaf;
		ret.toLeaf = fromLeaf;
		ret.fromMapIndices = toMapIndices;
		ret.toMapIndices = fromMapIndices;
		ret.mapIndices = mapIndices;
		return ret;
	}
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const OnionPortalInfo& rhs);

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d,OnionPortalInfo> OnionPortal;
typedef shared_ptr<OnionPortal> OnionPortal_Ptr;

}

#endif
