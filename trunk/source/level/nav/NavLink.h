/***
 * hesperus: NavLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_NAVLINK
#define H_HESP_LEVEL_NAV_NAVLINK

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class NavLink
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~NavLink() {}

	//#################### PUBLIC METHODS ####################
public:
	// TODO: Vector3d traverse(Vector3d oldPosition)
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavLink> NavLink_Ptr;
typedef shared_ptr<const NavLink> NavLink_CPtr;

}

#endif
