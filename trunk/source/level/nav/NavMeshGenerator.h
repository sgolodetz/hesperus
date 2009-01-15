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
		int polyIndex;		// the index of the nav polygon in which this edge resides
		int startVertex;

		EdgeReference(int polyIndex_, int startVertex_) : polyIndex(polyIndex_), startVertex(startVertex_) {}
	};

	typedef std::vector<EdgeReference> EdgeReferences;

	struct EdgeReferencesPair
	{
		EdgeReferences sameFacing;	// the edge planes for these edges face the same way as the undirected edge planes
		EdgeReferences oppFacing;	// the edge planes for these edges face the opposite way to the undirected edge planes
	};

	struct LinkIntervals
	{
		// TODO: We can add jump down and jump up intervals here if we want.
		Interval_Ptr stepDownInterval;
		Interval_Ptr stepUpInterval;
		Interval_Ptr walkInterval;
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
	LinkIntervals calculate_link_intervals(const Vector2d& s1, const Vector2d& s2, const Vector2d& d1, const Vector2d& d2, const Interval& xOverlap) const;
	void determine_links();
	static Plane make_edge_plane(const Vector3d& p1, const Vector3d& p2);
};

}

#endif
