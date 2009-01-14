/***
 * hesperus: NavPolygon.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_NAVPOLYGON
#define H_HESP_LEVEL_NAV_NAVPOLYGON

#include <vector>

#include <source/util/PolygonTypes.h>
#include "NavLink.h"

namespace hesp {

//#################### CLASSES ####################
class NavPolyAuxData : public ColPolyAuxData
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_leafIndex;					// in which leaf the polygon resides
	std::vector<NavLink_Ptr> m_links;	// navigation links to other polygons

	//#################### CONSTRUCTORS ####################
public:
	NavPolyAuxData(const ColPolyAuxData& rhs, int leafIndex);

	//#################### PUBLIC METHODS ####################
public:
	void add_link(const NavLink_Ptr& link);
};

//#################### TYPEDEFS ####################
typedef Polygon<Vector3d, NavPolyAuxData> NavPolygon;
typedef shared_ptr<NavPolygon> NavPolygon_Ptr;
typedef shared_ptr<const NavPolygon> NavPolygon_CPtr;

}

#endif
