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
		int polyIndex, startVertex;
		Plane plane;

		EdgeReference(int polyIndex_, int startVertex_, const Plane& plane_)
		:	polyIndex(polyIndex_), startVertex(startVertex_), plane(plane_) {}
	};

	//#################### TYPEDEFS ####################
private:
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	typedef std::vector<NavPolygon_Ptr> NavPolyVector;
	typedef std::vector<EdgeReference> EdgeReferences;
	typedef std::map<Plane,EdgeReferences,UniquePlanePred> EdgePlaneTable;

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
