/***
 * hesperus: GeomUtil.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "GeomUtil.h"

namespace hesp {

//#################### GLOBAL FUNCTIONS ####################
/**
Classifies the line segment p1-p2 against the plane.

@param p1		One endpoint of the line segment
@param p2		The other endpoint of the line segment
@param plane	The plane against which to classify the line segment
@param cp1		Used to return the classification of p1 against the plane to the caller
@param cp2		Used to return the classification of p2 against the plane to the caller
@return			CP_BACK, if the line segment is behind the plane; CP_COPLANAR, if it is on the plane; CP_FRONT, if it is in front of the plane;
				CP_STRADDLE, if it straddles the plane
*/
PlaneClassifier classify_linesegment_against_plane(const Vector3d& p1, const Vector3d& p2, const Plane& plane, PlaneClassifier& cp1, PlaneClassifier& cp2)
{
	cp1 = classify_point_against_plane(p1, plane);
	cp2 = classify_point_against_plane(p2, plane);

	bool backFlag = false, frontFlag = false;
	if(cp1 == CP_BACK || cp2 == CP_BACK) backFlag = true;
	if(cp1 == CP_FRONT || cp2 == CP_FRONT) frontFlag = true;

	if(backFlag && frontFlag) return CP_STRADDLE;
	else if(backFlag) return CP_BACK;
	else if(frontFlag) return CP_FRONT;
	else return CP_COPLANAR;
}

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
Calculates the perpendicular displacement of the point p from the plane.

@param p		The point whose perpendicular displacement from the plane we wish to determine
@param plane	The plane
@return			The perpendicular displacement of p from the plane
*/
double displacement_from_plane(const Vector3d& p, const Plane& plane)
{
	const Vector3d& n = plane.normal();
	double d = plane.distance_value();

	// Note that this equation is valid precisely because the plane normal is guaranteed to be unit length.
	return n.dot(p) - d;
}

/**
Calculates the perpendicular distance between the point p and the plane.

@param p		The point whose perpendicular distance from the plane we wish to determine
@param plane	The plane
@return			The perpendicular distance between p and the plane
*/
double distance_to_plane(const Vector3d& p, const Plane& plane)
{
	return fabs(displacement_from_plane(p, plane));
}

/**
Returns an arbitrary unit vector perpendicular to the specified plane's normal.

@param plane	The plane to which our arbitrary unit vector is to be coplanar
@return			An arbitrary unit vector v such that v.dot(plane.normal()) == 0
*/
Vector3d generate_arbitrary_coplanar_unit_vector(const Plane& plane)
{
	const Vector3d& n = plane.normal();
	Vector3d up(0,0,1);
	if(fabs(n.x) < EPSILON && fabs(n.y) < EPSILON)
	{
		// Special Case: n is too close to the vertical and hence n x up is roughly equal to (0,0,0)

		// Use a different vector instead of up (any different vector will do) and apply the same
		// method as in the else clause using the new vector.
		return n.cross(Vector3d(1,0,0)).normalize();
	}
	else
	{
		// The normalized cross product of n and up satisfies the requirements of being
		// unit length and perpendicular to n (since we dealt with the special case where
		// n x up is zero, in all other cases it must be non-zero and we can normalize it
		// to give us a unit vector)
		return n.cross(up).normalize();
	}
}

/**
Returns the point in the specified plane which is nearest to the specified point.

@param p		The point whose nearest point in the plane we want to find
@param plane	The plane
@return			A point r s.t. classify_point_against_plane(r, plane) == CP_COPLANAR and there
				is no point r' s.t. classify_point_against_plane(r', plane) == CP_COPLANAR &&
				p.distance(r') < p.distance(r)
*/
Vector3d nearest_point_in_plane(const Vector3d& p, const Plane& plane)
{
	/*
	Derivation of the algorithm:

	The nearest point in the plane is the point we get if we head from p towards the plane
	along the normal.
	*/

	double displacement = displacement_from_plane(p, plane);
	return p - displacement * plane.normal();
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
