/***
 * hesperus: NavPolygon.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavPolygon.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavPolygon::NavPolygon(int polyIndex, int leafIndex)
:	m_polyIndex(polyIndex), m_leafIndex(leafIndex)
{}

//#################### PUBLIC METHODS ####################
int NavPolygon::leaf_index() const	{ return m_leafIndex; }
int NavPolygon::poly_index() const	{ return m_polyIndex; }

}
