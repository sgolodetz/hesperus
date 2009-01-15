/***
 * hesperus: NavMesh.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_NAVMESH
#define H_HESP_LEVEL_NAV_NAVMESH

#include "NavPolygon.h"

namespace hesp {

class NavMesh
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<NavPolygon_Ptr> NavPolyVector;

	//#################### PRIVATE VARIABLES ####################
private:
	NavPolyVector m_polygons;

	//#################### CONSTRUCTORS ####################
public:
	NavMesh(const NavPolyVector& polygons);

	//#################### PUBLIC METHODS ####################
public:
	const NavPolyVector& polygons() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavMesh> NavMesh_Ptr;
typedef shared_ptr<const NavMesh> NavMesh_CPtr;

}

#endif
