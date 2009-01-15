/***
 * hesperus: NavPolygon.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_NAVPOLYGON
#define H_HESP_LEVEL_NAV_NAVPOLYGON

#include <vector>

#include <source/util/PolygonTypes.h>
#include "NavLink.h"

namespace hesp {

class NavPolygon
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_colPolyIndex;					// the index of the collision polygon on which this is based
	std::vector<NavLink_Ptr> m_links;	// navigation links to other polygons

	//#################### CONSTRUCTORS ####################
public:
	explicit NavPolygon(int colPolyIndex);

	//#################### PUBLIC METHODS ####################
public:
	void add_link(const NavLink_Ptr& link);
	int collision_poly_index() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavPolygon> NavPolygon_Ptr;
typedef shared_ptr<const NavPolygon> NavPolygon_CPtr;

}

#endif
