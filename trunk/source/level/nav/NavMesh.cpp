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
void NavMesh::build_collision_to_nav_lookup()
{
	int polyCount = static_cast<int>(m_polygons.size());
	for(int i=0; i<polyCount; ++i)
	{
		m_colToNavLookup.insert(std::make_pair(m_polygons[i]->collision_poly_index(), i));
	}
}

const NavMesh::NavLinkVector& NavMesh::links() const
{
	return m_links;
}

int NavMesh::lookup_nav_poly_index(int collisionPolyIndex) const
{
	std::map<int,int>::const_iterator it = m_colToNavLookup.find(collisionPolyIndex);
	if(it != m_colToNavLookup.end()) return it->second;
	else throw Exception("NavMesh::lookup_nav_poly_index(): Collision polygon index not found in map");
}

const NavMesh::NavPolyVector& NavMesh::polygons() const
{
	return m_polygons;
}

}
