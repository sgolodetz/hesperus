/***
 * hesperus: GeomUtil.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cmath>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

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
Constructs a bounding box around an array of polygons.

@param polys	The polygons
@return			The bounding box
*/
template <typename Vert, typename AuxData>
AABB3d construct_bounding_box(const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polys)
{
	Vector3d minimum(INT_MAX, INT_MAX, INT_MAX), maximum(INT_MIN, INT_MIN, INT_MIN);

	int polyCount = static_cast<int>(polys.size());
	for(int i=0; i<polyCount; ++i)
	{
		int vertCount = polys[i]->vertex_count();
		for(int j=0; j<vertCount; ++j)
		{
			const Vector3d& v = polys[i]->vertex(j);
			if(v.x < minimum.x) minimum.x = v.x;	if(v.x > maximum.x) maximum.x = v.x;
			if(v.y < minimum.y) minimum.y = v.y;	if(v.y > maximum.y) maximum.y = v.y;
			if(v.z < minimum.z) minimum.z = v.z;	if(v.z > maximum.z) maximum.z = v.z;
		}
	}

	return AABB3d(minimum, maximum);
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
Finds the unique set of planes in which the specified polygons lie.
Note that planes which differ only in the orientation of their
normal are treated as equivalent here.

@param polygons	The polygons whose unique set of planes we wish to find
@return			As stated
*/
template <typename Poly>
shared_ptr<std::list<Plane> > find_unique_planes(const std::vector<shared_ptr<Poly> >& polygons)
{
	typedef std::list<Plane> PlaneList;
	typedef shared_ptr<PlaneList> PlaneList_Ptr;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	const double angleTolerance = 0.5 * PI / 180;	// convert 0.5 degrees to radians
	const double distTolerance = 0.001;
	std::set<Plane, UniquePlanePred> planes(UniquePlanePred(angleTolerance, distTolerance));

	for(PolyVector::const_iterator it=polygons.begin(), iend=polygons.end(); it!=iend; ++it)
	{
		Plane plane = make_plane(**it).to_undirected_form();
		planes.insert(plane);
	}

	return PlaneList_Ptr(new PlaneList(planes.begin(), planes.end()));
}

/**
Loads a polygon from a std::string (generally a line of text taken from a file).

@param line	The std::string containing the polygon definition
@param n	The number of the line in the file (if available), as a std::string
@return		The polygon
*/
template <typename Vert, typename AuxData>
shared_ptr<Polygon<Vert,AuxData> > load_polygon(const std::string& line, const std::string& n)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	// Read the vertex count.
	std::string::size_type L = line.find(' ');
	if(L == std::string::npos) throw Exception("Bad input on line " + n);
	std::string vertCountString = line.substr(0,L);
	int vertCount;
	try								{ vertCount = boost::lexical_cast<int,std::string>(vertCountString); }
	catch(boost::bad_lexical_cast&)	{ throw Exception("Bad vertex count on line " + n); }

	// Read the auxiliary data.
	std::string::size_type R = line.find_last_of(')');
	if(R == std::string::npos || R+2 >= line.length()) throw Exception("Bad input on line " + n);
	std::string auxDataString = line.substr(R+2);
	boost::trim(auxDataString);
	AuxData auxData;
	try								{ auxData = boost::lexical_cast<AuxData,std::string>(auxDataString); }
	catch(boost::bad_lexical_cast&)	{ throw Exception("Bad auxiliary data on line " + n); }

	// Read the vertices.
	std::string verticesString = line.substr(L+1, R-L);
	std::vector<Vert> vertices;

	typedef boost::char_separator<char> sep;
	typedef boost::tokenizer<sep> tokenizer;
	tokenizer tok(verticesString.begin(), verticesString.end(), sep(" "));
	std::vector<std::string> tokens(tok.begin(), tok.end());
	int tokensPerVert = static_cast<int>(tokens.size()) / vertCount;
	if(tokensPerVert < 3) throw Exception("Bad vertex data on line " + n);

	for(int i=0; i<vertCount; ++i)
	{
		int offset = i*tokensPerVert;
		if(tokens[offset] != "(" || tokens[offset+tokensPerVert-1] != ")") throw Exception("Bad vertex data on line " + n);

		std::vector<std::string> components(&tokens[offset+1], &tokens[offset+tokensPerVert-1]);
		try					{ vertices.push_back(Vert(components)); }
		catch(Exception& e)	{ throw Exception(e.cause()); }
	}

	return Poly_Ptr(new Poly(vertices, auxData));
}

/**
Loads a sequence of polygons from a stream, one per line.

@param is		The stream from which to load
@param polygons	The std::vector into which to write the loaded polygons
*/
template <typename Vert, typename AuxData>
void load_polygons(std::istream& is, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, int maxToRead)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	std::string line;
	int n = 1;
	while(std::getline(is, line))
	{
		boost::trim(line);
		if(line != "")
		{
			polygons.push_back(load_polygon<Vert,AuxData>(line, boost::lexical_cast<std::string,int>(n)));
		}

		++n;
		if(n > maxToRead) break;
	}
}

/**
Makes a Plane object to represent the plane in which the specified polygon lies.

@poly	The polygon whose plane we wish to make
@return	The plane
*/
template <typename Vert, typename AuxData>
Plane make_plane(const Polygon<Vert,AuxData>& poly)
{
	return Plane(poly.normal(), poly.vertex(0));
}

/**
Returns a polygon on the specified plane which is large enough to
span the entire level space.

@param plane	The plane for which we want to make a universe polygon
@return			A polygon poly s.t. classify_polygon_against_plane(poly, plane) == CP_COPLANAR
				and the angle between the polygon and plane normals is less than EPSILON and
				poly is large enough to span the entire level space
*/
template <typename AuxData>
shared_ptr<Polygon<Vector3d,AuxData> > make_universe_polygon(const Plane& plane, const AuxData& auxData)
{
	typedef Polygon<Vector3d,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	Vector3d origin(0,0,0);
	Vector3d centre = nearest_point_in_plane(origin, plane);

	Vector3d planarVecs[2];
	planarVecs[0] = generate_arbitrary_coplanar_unit_vector(plane);
	planarVecs[1] = planarVecs[0].cross(plane.normal()).normalize();

	const double HALFSIDELENGTH = 1000000;	// something arbitrarily huge (but not too big, to avoid floating-point issues)
	for(int i=0; i<2; ++i) planarVecs[i] *= HALFSIDELENGTH;

	std::vector<Vector3d> vertices;
	for(int i=0; i<4; ++i) vertices.push_back(centre);
	vertices[0] -= planarVecs[0];	vertices[0] -= planarVecs[1];
	vertices[1] -= planarVecs[0];	vertices[1] += planarVecs[1];
	vertices[2] += planarVecs[0];	vertices[2] += planarVecs[1];
	vertices[3] += planarVecs[0];	vertices[3] -= planarVecs[1];

	return Poly_Ptr(new Poly(vertices, auxData));
}

/**
Determines whether or not p is inside poly (including its edges).

@param p		A point in the plane of poly
@param poly		A polygon
@return			true, if p is inside poly, or false otherwise
*/
template <typename Vert, typename AuxData>
bool point_in_polygon(const Vector3d& p, const Polygon<Vert,AuxData>& poly)
{
	assert(classify_point_against_plane(p, make_plane(poly)) == CP_COPLANAR);

	const Vector3d zero(0,0,0);

	std::vector<Vector3d> relVecs;	// position of vertices relative to p
	int vertCount = poly.vertex_count();
	relVecs.reserve(vertCount);
	for(int i=0; i<vertCount; ++i)
	{
		relVecs.push_back(Vector3d(poly.vertex(i)) - p);
		if(relVecs[i].length_squared() < EPSILON*EPSILON)
		{
			// p is one of the vertices of poly
			return true;
		}
		else relVecs[i].normalize();
	}

	double angleSum = 0;
	for(int i=0; i<vertCount; ++i)
	{
		int j = (i+1)%vertCount;
		double dotProd = relVecs[i].dot(relVecs[j]);

		// Implicitly clamp the result of the dot product to the range [-1,1] to avoid problems with rounding errors when we call acos.
		// Note that if dotProd = 1, acos(1) = 0, which is why this works.
		if(dotProd <= -1) angleSum += PI;
		else if(dotProd < 1) angleSum += acos(relVecs[i].dot(relVecs[j]));
	}

	return fabs(angleSum - 2*PI) < EPSILON;
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

/**
Writes a sequence of polygons to a stream, one per line.

@param os			The stream to which to write
@param polygons		The polygons to write to the stream
@param writeCount	Whether the polygon count should be output to the stream first
*/
template <typename Vert, typename AuxData>
void write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, bool writeCount)
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	if(writeCount) os << polygons.size() << '\n';
	for(PolyVector::const_iterator it=polygons.begin(), iend=polygons.end(); it!=iend; ++it)
	{
		const Poly& curPoly = **it;
		int vertCount = curPoly.vertex_count();
		os << vertCount << ' ';
		for(int j=0; j<vertCount; ++j)
		{
			os << curPoly.vertex(j) << ' ';
		}
		os << curPoly.auxiliary_data() << '\n';
	}
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
