/***
 * hesperus: Portal.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_PORTAL
#define H_HESP_BSP_PORTAL

#include <ostream>
#include <sstream>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/Polygon.h>

namespace hesp {

//#################### CLASSES ####################
struct PortalInfo
{
	int index;		// the index of the portal in a portal array (-1 if not being used)
	int fromLeaf;
	int toLeaf;

	PortalInfo() : index(-1), fromLeaf(-1), toLeaf(-1) {}

	PortalInfo(int fromLeaf_, int toLeaf_) : index(-1), fromLeaf(fromLeaf_), toLeaf(toLeaf_) {}

	bool operator==(const PortalInfo& rhs) const
	{
		return fromLeaf == rhs.fromLeaf && toLeaf == rhs.toLeaf;
	}
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const PortalInfo& rhs);
std::istream& operator>>(std::istream& is, PortalInfo& rhs);

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d,PortalInfo> Portal;
typedef shared_ptr<Portal> Portal_Ptr;

}

#endif
