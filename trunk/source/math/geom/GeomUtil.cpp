/***
 * hesperus: GeomUtil.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "GeomUtil.h"

namespace hesp {

//#################### GLOBAL FUNCTIONS ####################
/**
Classifies the point p against the plane.

@param p		The point to be classified
@param plane	The plane against which to classify it
@return			CP_BACK, if the point is behind the plane; CP_COPLANAR, if it is on the plane; CP_FRONT, if it is in front of the plane
*/
PlaneClassifier classify_point_against_plane(const Vector3d& p, const Plane& plane)
{
	const Vector3d& n = plane.normal();
	double d = plane.distance_value();

	double value = n.dot(p) - d;

	if(fabs(value) < EPSILON) return CP_COPLANAR;
	else if(value > 0) return CP_FRONT;
	else return CP_BACK;
}

/**
Calculates the perpendicular distance between the point p and the plane.

@param p		The point whose perpendicular distance from the plane we wish to determine
@param plane	The plane
@return			The perpendicular distance between p and the plane
*/
double distance_to_plane(const Vector3d& p, const Plane& plane)
{
	const Vector3d& n = plane.normal();
	double d = plane.distance_value();

	// Note that this equation is valid precisely because the plane normal is guaranteed to be unit length.
	return fabs(n.dot(p) - d);
}

}
