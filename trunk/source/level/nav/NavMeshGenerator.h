/***
 * hesperus: NavMeshGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_NAV_NAVMESHGENERATOR
#define H_HESP_LEVEL_NAV_NAVMESHGENERATOR

#include <map>

#include <source/level/onionbsp/OnionTree.h>
#include <source/math/geom/UniquePlanePred.h>
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

	//#################### TYPEDEFS ####################
private:
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	typedef std::vector<NavPolygon_Ptr> NavPolyVector;
	typedef std::map<Plane,EdgeReferencesPair,UniquePlanePred> EdgePlaneTable;

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	ColPolyVector m_polygons;
	OnionTree_CPtr m_tree;

	// Intermediate data
	EdgePlaneTable m_edgePlaneTable;

	// Output data
	NavPolyVector m_walkablePolygons;
	std::map<int,int> m_colToNavMap;

	//#################### CONSTRUCTORS ####################
public:
	NavMeshGenerator(const ColPolyVector& polygons, const OnionTree_Ptr& tree);

	//#################### PUBLIC METHODS ####################
public:
	void generate_mesh();

	//#################### PRIVATE METHODS ####################
private:
	void build_edge_plane_table();
	void determine_links();
	static Plane make_edge_plane(const Vector3d& p1, const Vector3d& p2);
};

}

#endif
