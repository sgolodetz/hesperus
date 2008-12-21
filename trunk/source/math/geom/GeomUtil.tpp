/***
 * hesperus: GeomUtil.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cmath>

#include <source/math/Constants.h>

namespace hesp {

//#################### GLOBAL FUNCTIONS ####################
/**
Classifies the polygon poly against the plane.

@param poly		The polygon to be classified
@param plane	The plane against which to classify it
@return			CP_BACK, if the polygon is entirely behind the plane; CP_COPLANAR, if it lies in the plane;
				CP_FRONT, if it is entirely in front of the plane; CP_STRADDLE, if it straddles the plane
*/
template <typename Vert, typename AuxData>
PlaneClassifier classify_polygon_against_plane(const Polygon<Vert,AuxData>& poly, const Plane& plane)
{
	bool backFlag = false, frontFlag = false;

	int vertCount = poly.vertex_count();
	for(int i=0; i<vertCount; ++i)
	{
		PlaneClassifier cp = classify_point_against_plane(poly.vertex(i), plane);
		switch(cp)
		{
		case CP_BACK:
			backFlag = true;
			break;
		case CP_FRONT:
			frontFlag = true;
			break;
		default:
			break;
		}
		if(backFlag && frontFlag) return CP_STRADDLE;
	}

	// If we get here, the polygon doesn't straddle the plane, so at most one of CP_BACK and CP_FRONT is true.
	if(backFlag) return CP_BACK;
	else if(frontFlag) return CP_FRONT;
	else return CP_COPLANAR;
}

/**
Splits the polygon across the specified plane.

@param poly		The polygon to split across the plane
@param plane	The plane across which to split the polygon
@return			A SplitResults object containing the results of the split
*/
template <typename Vert, typename AuxData>
SplitResults<Vert,AuxData> split_polygon_across_plane(const Polygon<Vert,AuxData>& poly, const Plane& plane)
{
	// NYI
	throw 23;
}

}
