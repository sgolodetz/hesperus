/***
 * hesperus: Portal.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_PORTAL
#define H_HESP_BSP_PORTAL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### CLASSES ####################
struct LeafLink
{
	int fromLeaf;
	int toLeaf;

	LeafLink() : fromLeaf(-1), toLeaf(-1) {}

	bool operator==(const LeafLink& rhs) const
	{
		return fromLeaf == rhs.fromLeaf && toLeaf == rhs.toLeaf;
	}
};

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d,LeafLink> Portal;
typedef shared_ptr<Portal> Portal_Ptr;

}

#endif
