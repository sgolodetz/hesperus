/***
 * hesperus: GeomUtil.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cmath>

#include <source/exceptions/InvalidParameterException.h>
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
Determines the point at which the line r = s + tv intersects the specified plane.

<p><b>Preconditions:</b>
<dl>
<dd>The line must not be parallel to the plane
<dd>|v| > 0
</dl>

@param s			An arbitrary point on the line
@param v			The direction vector of the line
@param plane		The plane with which to intersect
@return				A pair containing the intersection point of the line with the plane and the
					value of the parameter t at that intersection point
@throws Exception	If the line is parallel to the plane or |v| = 0
*/
template <typename Vec>
std::pair<Vec,double> determine_line_intersection_with_plane(const Vec& s, const Vec& v, const Plane& plane)
{
	/*
	Derivation of the algorithm:

	The plane's equation is n . x - d = 0
	The line's equation is r = s + tv

	We want to find where they intersect, i.e. we want to find t (and hence r) s.t.

	n . r - d = 0

	Well:

	n . (s + tv) - d = 0
	n . s + t(n . v) - d = 0
	t = (d - n . s) / n . v
	r = s + ((d - n . s) / n . v).v

	Of course, there's only a solution if n . v != 0, i.e. if the line isn't parallel to the plane and |v| > 0
	*/

	const Vector3d& n = plane.normal();
	double d = plane.distance_value();

	double nDOTv = n.dot(v);

	// If the line is parallel to the plane or |v| = 0, we've violated our precondition.
	if(nDOTv == 0) throw Exception("n . v == 0");

	double t = (d - n.dot(s)) / nDOTv;
	Vec tv = t*v;
	Vec r = s + tv;
	return std::make_pair(r,t);
}

/**
Determines the point at which the line segment p1-p2 intersects the specified plane.

<p><b>Preconditions:</b>
<dl>
<dd>The line segment must not be parallel to the plane
<dd>The two points must not be identical
</dl>

@param p1			One endpoint of the line segment
@param p2			The other endpoint of the line segment
@param plane		The plane with which to intersect
@return				A pair containing the intersection point of the line in which the line segment lies with
					the plane and a boolean indicating whether or not it was (strictly) between the endpoints
@throws Exception	If the line segment is parallel to the plane or the points are identical
*/
template <typename Vec>
std::pair<Vec,bool> determine_linesegment_intersection_with_plane(const Vec& p1, const Vec& p2, const Plane& plane)
{
	/*
	Derivation of the algorithm:

	The intersection of a line segment with a plane can be calculated by intersecting the line in which it
	lies with the plane and checking whether the intersection point is between the specified endpoints of
	the segment. The intersection point returned is (strictly) between the endpoints iff the value of the
	parameter t returned by determine_line_intersection_with_plane satisfies 0 < t < 1.
	*/

	std::pair<Vec,double> intersection = determine_line_intersection_with_plane(p1, p2 - p1, plane);
	return std::make_pair(intersection.first, 0 < intersection.second && intersection.second < 1);
}

/**
Splits the polygon across the specified plane.

@param poly							The polygon to split across the plane
@param plane						The plane across which to split the polygon
@return								A SplitResults object containing the results of the split
@throws InvalidParameterException	If the polygon doesn't straddle the plane
*/
template <typename Vert, typename AuxData>
SplitResults<Vert,AuxData> split_polygon(const Polygon<Vert,AuxData>& poly, const Plane& plane)
{
	if(classify_polygon_against_plane(poly, plane) != CP_STRADDLE) throw InvalidParameterException("Polygon doesn't straddle plane");

	std::vector<Vert> backHalf, frontHalf;

	int startVert = 0;
	PlaneClassifier cp = classify_point_against_plane(poly.vertex(0), plane);
	if(cp == CP_COPLANAR)	// special case: we've started at the beginning of one half of the polygon
	{
		cp = classify_point_against_plane(poly.vertex(1), plane);

		if(cp == CP_COPLANAR)
		{
			// If the polygon doesn't straddle the plane, we've violated a precondition, so throw an exception.
			throw InvalidParameterException("Polygon doesn't straddle plane");
		}
		else
		{
			// Otherwise, start from the next vertex along to avoid writing extra special case code.
			startVert = 1;
		}
	}

	// At this point, cp contains the classification of poly.vertex(startVert) against the plane
	// Note that cp != CP_COPLANAR

	switch(cp)
	{
		case CP_BACK:
		{
			std::pair<int,PlaneClassifier> p = construct_half_polygon(backHalf, frontHalf, poly, plane, startVert, cp);
			p = construct_half_polygon(frontHalf, backHalf, poly, plane, p.first, p.second);
			complete_original_half(backHalf, poly, p.first, startVert);
			break;
		}
		case CP_FRONT:
		{
			std::pair<int,PlaneClassifier> p = construct_half_polygon(frontHalf, backHalf, poly, plane, startVert, cp);
			p = construct_half_polygon(backHalf, frontHalf, poly, plane, p.first, p.second);
			complete_original_half(frontHalf, poly, p.first, startVert);
			break;
		}
	}

	// Construct the polygons from the lists of vertices and the auxiliary data.
	// Note that the auxiliary data is simply inherited from the parent polygon:
	// it may or may not be necessary to allow this behaviour to be customised
	// in the future.
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	Poly_Ptr backPoly(new Poly(backHalf, poly.auxiliary_data()));
	Poly_Ptr frontPoly(new Poly(frontHalf, poly.auxiliary_data()));

	return SplitResults<Vert,AuxData>(backPoly, frontPoly);
}

//################## HELPER METHODS FOR THE split_polygon FUNCTION ##################
/**
Completes the original half of the split polygon (the side we did first). (It's used, obviously, in the
split_polygon function.)

@param originalHalf	The vertex list for the side of the split polygon we constructed first
@param poly			The polygon we're splitting
@param i			The index of the vertex we're currently processing
@param startVert	The first vertex we processed
*/
template <typename Vert, typename AuxData>
void complete_original_half(std::vector<Vert>& originalHalf, const Polygon<Vert,AuxData>& poly, int i, int startVert)
{
	const int vertCount = poly.vertex_count();

	while(i != startVert)
	{
		originalHalf.push_back(poly.vertex(i));
		i = next_vert(i, vertCount);
	}
}

/**
Constructs one of the two halves of the split polygon. (It's used, obviously, in the split_polygon function.)

<p><b>Preconditions:</b>
<dl>
<dd>cp == CP_BACK or CP_FRONT (initially, at any rate - it does get modified)
<dd>currentHalf != otherHalf
<dd>0 <= i < poly.vertex_count()
</dl>

@param currentHalf	The vertices for the half of the polygon we're currently constructing
@param otherHalf	The vertices for the other half of the polygon
@param poly			The polygon we're splitting
@param plane		The splitting plane
@param i			The index of the vertex we're currently processing
@param cp			The classification of that vertex against the splitting plane
@return				A pair, the first component of which is the new value of i and the second of which is the new value of cp
*/
template <typename Vert, typename AuxData>
std::pair<int,PlaneClassifier> construct_half_polygon(std::vector<Vert>& currentHalf, std::vector<Vert>& otherHalf, const Polygon<Vert,AuxData>& poly,
													  const Plane& plane, int i, PlaneClassifier cp)
{
	const int vertCount = poly.vertex_count();

	PlaneClassifier oldcp;	// stores the classification of the previous vertex against the plane (initially indeterminate)
	do
	{
		currentHalf.push_back(poly.vertex(i));
		i = next_vert(i, vertCount);
		oldcp = cp;
		cp = classify_point_against_plane(poly.vertex(i), plane);
	} while(cp == oldcp);

	// If two consecutive points aren't on the same side of the plane and the first one
	// isn't coplanar, either the second one's coplanar or it's on the opposite side of
	// the plane from the first.
	if(cp == CP_COPLANAR)
	{
		currentHalf.push_back(poly.vertex(i));
		otherHalf.push_back(poly.vertex(i));
		i = next_vert(i, vertCount);
		cp = classify_point_against_plane(poly.vertex(i), plane);

		// Having two coplanar points in sequence would imply that the polygon doesn't
		// straddle the plane, which violates our precondition.
		if(cp == CP_COPLANAR) throw InvalidParameterException("Polygon doesn't straddle plane");
	}
	else
	{
		Vert intersectionPoint = determine_linesegment_intersection_with_plane(poly.vertex(prev_vert(i, vertCount)), poly.vertex(i), plane).first;
		currentHalf.push_back(intersectionPoint);
		otherHalf.push_back(intersectionPoint);
	}

	// Note: At this point, the vertex with index i is in the other half of the polygon
	// (i.e. non-coplanar). Its classification against the plane is cp.
	return std::make_pair(i, cp);
}

}
