/***
 * hesperus: NavMesh.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVMESH
#define H_HESP_NAVMESH

#include "NavPolygon.h"

namespace hesp {

class NavMesh
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<NavLink_Ptr> NavLinkVector;
	typedef std::vector<NavPolygon_Ptr> NavPolyVector;

	//#################### PRIVATE VARIABLES ####################
private:
	NavLinkVector m_links;
	NavPolyVector m_polygons;

	//#################### CONSTRUCTORS ####################
public:
	NavMesh(const NavPolyVector& polygons, const NavLinkVector& links);

	//#################### PUBLIC METHODS ####################
public:
	const NavLinkVector& links() const;
	const NavPolyVector& polygons() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavMesh> NavMesh_Ptr;
typedef shared_ptr<const NavMesh> NavMesh_CPtr;

}

#endif
