/***
 * hesperus: NavMesh.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavMesh.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavMesh::NavMesh(const NavPolyVector& polygons)
:	m_polygons(polygons)
{}

//#################### PUBLIC METHODS ####################
const NavMesh::NavPolyVector& NavMesh::polygons() const
{
	return m_polygons;
}

}
