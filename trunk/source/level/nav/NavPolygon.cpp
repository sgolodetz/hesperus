/***
 * hesperus: NavPolygon.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavPolygon.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavPolygon::NavPolygon(int polyIndex)
:	m_polyIndex(polyIndex)
{}

//#################### PUBLIC METHODS ####################
int NavPolygon::poly_index() const	{ return m_polyIndex; }

}
