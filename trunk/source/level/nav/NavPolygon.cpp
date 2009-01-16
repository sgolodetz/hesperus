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
void NavPolygon::add_link_index(int linkIndex)				{ m_linkIndices.push_back(linkIndex); }
int NavPolygon::collision_poly_index() const				{ return m_colPolyIndex; }
const std::vector<int>& NavPolygon::link_indices() const	{ return m_linkIndices; }

}
