/***
 * hesperus: NavMeshGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavMeshGenerator.h"

// TEMPORARY
#include <iostream>

#include <source/math/Constants.h>
#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
NavMeshGenerator::NavMeshGenerator(const ColPolyVector& polygons)
:	m_polygons(polygons), m_edgePlaneTable(UniquePlanePred(2 * PI/180, 0.005))
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
			Plane undirectedEdgePlane = edgePlane.to_undirected_form();
			bool sameFacing = edgePlane.normal().dot(undirectedEdgePlane.normal()) > 0;
			if(sameFacing)
				m_edgePlaneTable[undirectedEdgePlane].sameFacing.push_back(EdgeReference(i,j));
			else
				m_edgePlaneTable[undirectedEdgePlane].oppFacing.push_back(EdgeReference(i,j));
		}
	}
}

void NavMeshGenerator::determine_links()
{
	for(EdgePlaneTable::const_iterator it=m_edgePlaneTable.begin(), iend=m_edgePlaneTable.end(); it!=iend; ++it)
	{
		// Generate (n,u,v) coordinate system for plane.
		const Plane& plane = it->first;
		Vector3d origin(0,0,0);
		Vector3d centre = nearest_point_in_plane(origin, plane);
		const Vector3d& n = plane.normal();
		Vector3d u = generate_specific_coplanar_unit_vector(plane);
		Vector3d v = u.cross(n).normalize();

		// Check pairs of different-facing edges to see whether we need to create any links.
		const EdgeReferences& sameFacingEdgeRefs = it->second.sameFacing;
		const EdgeReferences& oppFacingEdgeRefs = it->second.oppFacing;
		int sameFacingEdgeRefCount = static_cast<int>(sameFacingEdgeRefs.size());
		int oppFacingEdgeRefCount = static_cast<int>(oppFacingEdgeRefs.size());

		for(int j=0; j<sameFacingEdgeRefCount; ++j)
		{
			const EdgeReference& edgeJ = sameFacingEdgeRefs[j];
			const NavPolygon& navPolyJ = *m_walkablePolygons[edgeJ.polyIndex];
			const CollisionPolygon& colPolyJ = *m_polygons[navPolyJ.poly_index()];
			int vertCountJ = colPolyJ.vertex_count();
			int mapIndexJ = colPolyJ.auxiliary_data().map_index();
			const Vector3d& p1J = colPolyJ.vertex(edgeJ.startVertex);	const Vector3d& p2J = colPolyJ.vertex((edgeJ.startVertex+1)%vertCountJ);
			Vector2d q1J((p1J-centre).dot(u), (p1J-centre).dot(v));		Vector2d q2J((p2J-centre).dot(u), (p2J-centre).dot(v));

			for(int k=0; k<oppFacingEdgeRefCount; ++k)
			{
				const EdgeReference& edgeK = oppFacingEdgeRefs[k];
				const NavPolygon& navPolyK = *m_walkablePolygons[edgeK.polyIndex];
				const CollisionPolygon& colPolyK = *m_polygons[navPolyK.poly_index()];
				int vertCountK = colPolyK.vertex_count();
				int mapIndexK = colPolyK.auxiliary_data().map_index();
				const Vector3d& p1K = colPolyK.vertex(edgeK.startVertex);	const Vector3d& p2K = colPolyK.vertex((edgeK.startVertex+1)%vertCountK);
				Vector2d q1K((p1K-centre).dot(u), (p1K-centre).dot(v));		Vector2d q2K((p2K-centre).dot(u), (p2K-centre).dot(v));

				// We only want to create links between polygons in the same map.
				if(mapIndexJ != mapIndexK) continue;

				// TODO

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
