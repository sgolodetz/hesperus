/***
 * hesperus: NavMeshGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavMeshGenerator.h"

// TEMPORARY
#include <iostream>

#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
NavMeshGenerator::NavMeshGenerator(const ColPolyVector& polygons, const OnionTree_Ptr& tree)
:	m_polygons(polygons), m_tree(tree), m_edgePlaneTable(UniquePlanePred(2 * PI/180, 0.005))
{
	int polyCount = static_cast<int>(polygons.size());

	// Construct the walkable polygon array.
	for(int i=0; i<polyCount; ++i)
	{
		if(m_polygons[i]->auxiliary_data().walkable())
		{
			m_colToNavMap[i] = static_cast<int>(m_walkablePolygons.size());
			m_walkablePolygons.push_back(NavPolygon_Ptr(new NavPolygon(i)));
		}
	}
}

//#################### PUBLIC METHODS ####################
void NavMeshGenerator::generate_mesh()
{
	build_edge_plane_table();
	determine_links();
}

//#################### PRIVATE METHODS ####################
void NavMeshGenerator::build_edge_plane_table()
{
	int walkablePolyCount = static_cast<int>(m_walkablePolygons.size());

	for(int i=0; i<walkablePolyCount; ++i)
	{
		const CollisionPolygon& curPoly = *m_polygons[m_walkablePolygons[i]->poly_index()];
		int vertCount = curPoly.vertex_count();
		for(int j=0; j<vertCount; ++j)
		{
			int k = (j+1)%vertCount;
			const Vector3d& p1 = curPoly.vertex(j);
			const Vector3d& p2 = curPoly.vertex(k);
			Plane edgePlane = make_edge_plane(p1,p2);
			m_edgePlaneTable[edgePlane.to_undirected_form()].push_back(EdgeReference(i,j,edgePlane));
		}
	}
}

void NavMeshGenerator::determine_links()
{
	for(EdgePlaneTable::const_iterator it=m_edgePlaneTable.begin(), iend=m_edgePlaneTable.end(); it!=iend; ++it)
	{
		// TODO: Generate coordinate system for plane.

		const EdgeReferences& edgeRefs = it->second;
		int edgeRefCount = static_cast<int>(edgeRefs.size());
		for(int j=0; j<edgeRefCount-1; ++j)
		{
			const EdgeReference& edgeJ = edgeRefs[j];
			const NavPolygon& polyJ = *m_walkablePolygons[edgeJ.polyIndex];
			int mapIndexJ = m_polygons[polyJ.poly_index()]->auxiliary_data().map_index();

			for(int k=j+1; k<edgeRefCount; ++k)
			{
				const EdgeReference& edgeK = edgeRefs[k];
				const NavPolygon& polyK = *m_walkablePolygons[edgeK.polyIndex];
				int mapIndexK = m_polygons[polyK.poly_index()]->auxiliary_data().map_index();

				// We only want to create links between polygons in the same map.
				if(mapIndexJ != mapIndexK) continue;

				// We can early-out if the two polygons are on the same side of the plane.
				// This will always be the case if the normals of the edge planes point in
				// the same direction.
				double dotProd = edgeJ.plane.normal().dot(edgeK.plane.normal());
				if(dotProd > 0) continue;

				std::cout << "Possible link between walkable polygons " << edgeJ.polyIndex << " and " << edgeK.polyIndex << " on plane " << it->first << " in map " << mapIndexJ << '\n';
			}
		}
	}
}

Plane NavMeshGenerator::make_edge_plane(const Vector3d& p1, const Vector3d& p2)
{
	// FIXME:	This is very similar to BrushExpander::make_bevel_plane(). It's a good idea to
	//			factor out the commonality at some point.
	Vector3d u = (p2 - p1).normalize();
	Vector3d v(0,0,1);
	Vector3d n = u.cross(v);

	if(n.length_squared() < EPSILON*EPSILON)
		throw Exception("Bad input to the navigation mesh generator: one of the polygons was vertical");

	return Plane(n, p1);
}

}
