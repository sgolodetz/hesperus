/***
 * hesperus: NavMesh.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavMesh.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavMesh::NavMesh(const NavPolyVector& polygons, const NavLinkVector& links)
:	m_polygons(polygons), m_links(links)
{}

//#################### PUBLIC METHODS ####################
const NavMesh::NavLinkVector& NavMesh::links() const
{
	return m_links;
}

const NavMesh::NavPolyVector& NavMesh::polygons() const
{
	return m_polygons;
}

}
