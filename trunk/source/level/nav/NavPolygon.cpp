/***
 * hesperus: NavPolygon.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavPolygon.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavPolyAuxData::NavPolyAuxData(const ColPolyAuxData& rhs, int leafIndex)
:	ColPolyAuxData(rhs.walkable()), m_leafIndex(leafIndex)
{}

//#################### PUBLIC METHODS ####################
void NavPolyAuxData::add_link(const NavLink_Ptr& link)
{
	// NYI
	throw 23;
}

}
