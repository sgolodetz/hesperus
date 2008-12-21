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

//################## HELPER METHODS FOR THE split_polygon FUNCTION ##################
/**
Returns the index of the next vertex (in winding order) after the one specified.

@param index		Some vertex index
@param vertCount	The number of vertices in the polygon
@return				The index of the next vertex after the one specified
*/
int next_vert(int index, int vertCount)
{
	return (index+1)%vertCount;
}

/**
Returns the index of the previous vertex (in winding order) before the one specified.

@param index		Some vertex index
@param vertCount	The number of vertices in the polygon
@return				The index of the previous vertex before the one specified
*/
int prev_vert(int index, int vertCount)
{
	return (index+vertCount-1)%vertCount;
}

}
