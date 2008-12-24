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
struct LeafLink
{
	int fromLeaf;
	int toLeaf;

	LeafLink() : fromLeaf(-1), toLeaf(-1) {}

	LeafLink(int fromLeaf_, int toLeaf_) : fromLeaf(fromLeaf_), toLeaf(toLeaf_) {}

	bool operator==(const LeafLink& rhs) const
	{
		return fromLeaf == rhs.fromLeaf && toLeaf == rhs.toLeaf;
	}
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const LeafLink& rhs);
std::istream& operator>>(std::istream& is, LeafLink& rhs);

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d,LeafLink> Portal;
typedef shared_ptr<Portal> Portal_Ptr;

}

#endif
