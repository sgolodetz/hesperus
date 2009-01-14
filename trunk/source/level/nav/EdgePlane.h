/***
 * hesperus: EdgePlane.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_EDGEPLANE
#define H_HESP_LEVEL_NAV_EDGEPLANE

#include <source/math/geom/Plane.h>
#include <source/math/vectors/Vector3.h>

namespace hesp {

class EdgePlane
{
	//#################### PRIVATE VARIABLES ####################
private:
	Plane_Ptr m_plane;
	int m_polyIndex;
	// TODO

	//#################### CONSTRUCTORS ####################
public:
	EdgePlane(int polyIndex, const Vector3d& p1, const Vector3d& p2);
};

}

#endif
