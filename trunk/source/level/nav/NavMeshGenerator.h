/***
 * hesperus: NavMeshGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_NAVMESHGENERATOR
#define H_HESP_LEVEL_NAV_NAVMESHGENERATOR

#include <map>

#include <source/level/onionbsp/OnionTree.h>
#include <source/math/Interval.h>
#include <source/math/geom/UniquePlanePred.h>
#include <source/math/vectors/Vector2d.h>
#include "NavPolygon.h"

namespace hesp {

class NavMeshGenerator
{
	//#################### NESTED CLASSES ####################
private:
	struct EdgeReference
	{
		int navPolyIndex;	// the index of the nav polygon in which this edge resides
		int startVertex;

		EdgeReference(int navPolyIndex_, int startVertex_) : navPolyIndex(navPolyIndex_), startVertex(startVertex_) {}
	};

	typedef std::vector<EdgeReference> EdgeReferences;

	struct EdgeReferencesPair
	{
		EdgeReferences sameFacing;	// the edge planes for these edges face the same way as the undirected edge planes
		EdgeReferences oppFacing;	// the edge planes for these edges face the opposite way to the undirected edge planes
	};

	struct LinkSegment
	{
		Vector2d p1, p2;

		LinkSegment(const Vector2d& p1_, const Vector2d& p2_) : p1(p1_), p2(p2_) {}
	};

	typedef shared_ptr<LinkSegment> LinkSegment_Ptr;

	struct LinkSegments
	{
		// TODO: We can add jump down and jump up segments here if we want.
		LinkSegment_Ptr stepDownSourceToDestSegment;
		LinkSegment_Ptr stepDownDestToSourceSegment;
		LinkSegment_Ptr stepUpSourceToDestSegment;
		LinkSegment_Ptr stepUpDestToSourceSegment;
		LinkSegment_Ptr walkSegment;
	};

	//#################### TYPEDEFS ####################
private:
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	typedef std::vector<NavPolygon_Ptr> NavPolyVector;
	typedef std::map<Plane,EdgeReferencesPair,UniquePlanePred> EdgePlaneTable;

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	ColPolyVector m_polygons;

	// Intermediate data
	EdgePlaneTable m_edgePlaneTable;

	// Output data
	NavPolyVector m_walkablePolygons;
	std::map<int,int> m_colToNavMap;

	//#################### CONSTRUCTORS ####################
public:
	NavMeshGenerator(const ColPolyVector& polygons);

	//#################### PUBLIC METHODS ####################
public:
	void generate_mesh();

	//#################### PRIVATE METHODS ####################
private:
	void build_edge_plane_table();
	LinkSegments calculate_link_segments(const Vector2d& s1, const Vector2d& s2, const Vector2d& d1, const Vector2d& d2, const Interval& xOverlap) const;
	void determine_links();
	static Plane make_edge_plane(const Vector3d& p1, const Vector3d& p2);
};

}

#endif
