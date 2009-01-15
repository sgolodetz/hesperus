/***
 * hesperus: NavPolygon.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavPolygon.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavPolygon::NavPolygon(int colPolyIndex)
:	m_colPolyIndex(colPolyIndex)
{}

//#################### PUBLIC METHODS ####################
void NavPolygon::add_link(const NavLink_Ptr& link)	{ m_links.push_back(link); }
int NavPolygon::collision_poly_index() const		{ return m_colPolyIndex; }

}
