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

//#################### LOCAL CLASSES ####################
class OrthonormalCoordSystem2D
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_o, m_n, m_u, m_v;

	//#################### CONSTRUCTORS ####################
public:
	OrthonormalCoordSystem2D(const Plane& plane)
	{
		Vector3d origin(0,0,0);
		m_o = nearest_point_in_plane(origin, plane);
		m_n = plane.normal();
		m_u = generate_specific_coplanar_unit_vector(plane);
		m_v = Vector3d(0,0,1);	// == m_u.cross(m_n).normalize();
	}

	//#################### PUBLIC METHODS ####################
public:
	Vector2d from_canonical(const Vector3d& x) const
	{
		return Vector2d((x-m_o).dot(m_u), (x-m_o).dot(m_v));
	}

	Vector3d to_canonical(const Vector2d& x) const
	{
		return m_o + x.x*m_u + x.y*m_v;
	}
};

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
		const CollisionPolygon& curPoly = *m_polygons[m_walkablePolygons[i]->collision_poly_index()];
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

NavMeshGenerator::LinkSegments
NavMeshGenerator::calculate_link_segments(const Vector2d& s1, const Vector2d& s2, const Vector2d& d1, const Vector2d& d2,
										  const Interval& xOverlap) const
{
	const double MAX_HEIGHT_DIFFERENCE = 1.0;	// FIXME: This depends on the player and should be a parameter.

	LinkSegments linkSegments;

	// Calculate the line equations yS = mS.x + cS and yD = mD.x + cD.
	assert(fabs(s2.x - s1.x) > EPSILON);
	assert(fabs(d2.x - d1.x) > EPSILON);

	double mS = (s2.y - s1.y) / (s2.x - s1.x);
	double mD = (d2.y - d1.y) / (d2.x - d1.x);
	double cS = s1.y - mS * s1.x;
	double cD = d1.y - mD * d1.x;

	double deltaM = mD - mS;
	double deltaC = cD - cS;

	if(fabs(deltaM) > EPSILON)
	{
		// We want to find:
		// (a) The point walkX at which yD = yS
		// (b) The point stepUpX at which yD - yS = MAX_HEIGHT_DIFFERENCE
		// (c) The point stepDownX at which yS - yD = MAX_HEIGHT_DIFFERENCE

		// (a) deltaM . walkX + deltaC = 0
		double walkX = -deltaC / deltaM;

		// (b) deltaM . stepUpX + deltaC = MAX_HEIGHT_DIFFERENCE
		double stepUpX = (MAX_HEIGHT_DIFFERENCE - deltaC) / deltaM;

		// (c) deltaM . stepDownX + deltaC = -MAX_HEIGHT_DIFFERENCE
		double stepDownX = (-MAX_HEIGHT_DIFFERENCE - deltaC) / deltaM;

		// Now construct the link intervals and clip them to the known x overlap interval.
		Interval stepDownInterval(std::min(walkX,stepDownX), std::max(walkX,stepDownX));
		Interval stepUpInterval(std::min(walkX,stepUpX), std::max(walkX,stepUpX));
		stepDownInterval = stepDownInterval.intersect(xOverlap);
		stepUpInterval = stepUpInterval.intersect(xOverlap);

		// Finally, construct the link segments from the link intervals.
		if(!stepDownInterval.empty())
		{
			Vector2d sL(stepDownInterval.low(), mS*stepDownInterval.low()+cS);
			Vector2d sH(stepDownInterval.high(), mS*stepDownInterval.high()+cS);
			linkSegments.stepDownSourceToDestSegment.reset(new LinkSegment(sL,sH));

			Vector2d dL(stepDownInterval.low(), mD*stepDownInterval.low()+cD);
			Vector2d dH(stepDownInterval.high(), mD*stepDownInterval.high()+cD);
			linkSegments.stepUpDestToSourceSegment.reset(new LinkSegment(dL,dH));
		}

		if(!stepUpInterval.empty())
		{
			Vector2d sL(stepUpInterval.low(), mS*stepUpInterval.low()+cS);
			Vector2d sH(stepUpInterval.high(), mS*stepUpInterval.high()+cS);
			linkSegments.stepUpSourceToDestSegment.reset(new LinkSegment(sL,sH));

			Vector2d dL(stepUpInterval.low(), mD*stepUpInterval.low()+cD);
			Vector2d dH(stepUpInterval.high(), mD*stepUpInterval.high()+cD);
			linkSegments.stepDownDestToSourceSegment.reset(new LinkSegment(dL,dH));
		}
	}
	else
	{
		// The lines are parallel.
		if(deltaC < MAX_HEIGHT_DIFFERENCE)
		{
			Vector2d p1(xOverlap.low(), mS*xOverlap.low()+cS);
			Vector2d p2(xOverlap.high(), mS*xOverlap.high()+cS);

			// There's a link between the lines, but we need to check the sign of deltaC to see which type.
			if(deltaC > SMALL_EPSILON)
			{
				// The destination is higher than the source: step up.
				linkSegments.stepUpSourceToDestSegment.reset(new LinkSegment(p1,p2));
				linkSegments.stepDownDestToSourceSegment.reset(new LinkSegment(p1,p2));
			}
			else if(deltaC < -SMALL_EPSILON)
			{
				// The destination is lower than the source: step down.
				linkSegments.stepDownSourceToDestSegment.reset(new LinkSegment(p1,p2));
				linkSegments.stepUpDestToSourceSegment.reset(new LinkSegment(p1,p2));
			}
			else	// |deltaC| < SMALL_EPSILON
			{
				// The destination and source are at the same level: just walk across.
				linkSegments.walkSegment.reset(new LinkSegment(p1,p2));
			}
		}
	}

	return linkSegments;
}

void NavMeshGenerator::determine_links()
{
	for(EdgePlaneTable::const_iterator it=m_edgePlaneTable.begin(), iend=m_edgePlaneTable.end(); it!=iend; ++it)
	{
		// Generate (n,u,v) coordinate system for plane, where v = (0,0,1).
		const Plane& plane = it->first;
		OrthonormalCoordSystem2D coordSystem(plane);

		// Check pairs of different-facing edges to see whether we need to create any links.
		const EdgeReferences& sameFacingEdgeRefs = it->second.sameFacing;
		const EdgeReferences& oppFacingEdgeRefs = it->second.oppFacing;
		int sameFacingEdgeRefCount = static_cast<int>(sameFacingEdgeRefs.size());
		int oppFacingEdgeRefCount = static_cast<int>(oppFacingEdgeRefs.size());

		for(int j=0; j<sameFacingEdgeRefCount; ++j)
		{
			const EdgeReference& edgeJ = sameFacingEdgeRefs[j];
			NavPolygon& navPolyJ = *m_walkablePolygons[edgeJ.navPolyIndex];
			const CollisionPolygon& colPolyJ = *m_polygons[navPolyJ.collision_poly_index()];
			int mapIndexJ = colPolyJ.auxiliary_data().map_index();

			// Calculate the 2D coordinates of edgeJ in the plane.
			const Vector3d& p1J = colPolyJ.vertex(edgeJ.startVertex);
			const Vector3d& p2J = colPolyJ.vertex((edgeJ.startVertex+1) % colPolyJ.vertex_count());
			Vector2d q1J = coordSystem.from_canonical(p1J), q2J = coordSystem.from_canonical(p2J);
			Interval xIntervalJ(std::min(q1J.x,q2J.x), std::max(q1J.x,q2J.x));

			for(int k=0; k<oppFacingEdgeRefCount; ++k)
			{
				const EdgeReference& edgeK = oppFacingEdgeRefs[k];
				NavPolygon& navPolyK = *m_walkablePolygons[edgeK.navPolyIndex];
				const CollisionPolygon& colPolyK = *m_polygons[navPolyK.collision_poly_index()];
				int mapIndexK = colPolyK.auxiliary_data().map_index();

				// We only want to create links between polygons in the same map.
				if(mapIndexJ != mapIndexK) continue;

				// Calculate the 2D coordinates of edgeK in the plane.
				const Vector3d& p1K = colPolyK.vertex(edgeK.startVertex);
				const Vector3d& p2K = colPolyK.vertex((edgeK.startVertex+1) % colPolyK.vertex_count());
				Vector2d q1K = coordSystem.from_canonical(p1K), q2K = coordSystem.from_canonical(p2K);

				// Calculate the x overlap between the 2D edges. If there's no overlap,
				// then we don't need to carry on looking for a link.
				Interval xIntervalK(std::min(q1K.x,q2K.x), std::max(q1K.x,q2K.x));
				Interval xOverlap = xIntervalJ.intersect(xIntervalK);
				if(xOverlap.empty()) continue;

				// Calculate the segments for the various types of link.
				LinkSegments linkSegments = calculate_link_segments(q1J, q2J, q1K, q2K, xOverlap);

				// Add the appropriate links.
				if(linkSegments.stepDownSourceToDestSegment)
				{
					assert(linkSegments.stepUpDestToSourceSegment != NULL);

					// Add a step down link from j -> k, and a step up one from k -> j.
					{
						Vector3d p1 = coordSystem.to_canonical(linkSegments.stepDownSourceToDestSegment->p1);
						Vector3d p2 = coordSystem.to_canonical(linkSegments.stepDownSourceToDestSegment->p2);
						navPolyJ.add_link(NavLink_Ptr(new StepDownLink(mapIndexJ, edgeK.navPolyIndex, p1, p2)));
					}
					{
						Vector3d p1 = coordSystem.to_canonical(linkSegments.stepUpDestToSourceSegment->p1);
						Vector3d p2 = coordSystem.to_canonical(linkSegments.stepUpDestToSourceSegment->p2);
						navPolyK.add_link(NavLink_Ptr(new StepUpLink(mapIndexJ, edgeJ.navPolyIndex, p1, p2)));
					}
				}
				if(linkSegments.stepUpSourceToDestSegment)
				{
					assert(linkSegments.stepDownDestToSourceSegment != NULL);

					// Add a step up link from j -> k, and a step down one from k -> j.
					{
						Vector3d p1 = coordSystem.to_canonical(linkSegments.stepUpSourceToDestSegment->p1);
						Vector3d p2 = coordSystem.to_canonical(linkSegments.stepUpSourceToDestSegment->p2);
						navPolyJ.add_link(NavLink_Ptr(new StepUpLink(mapIndexJ, edgeK.navPolyIndex, p1, p2)));
					}
					{
						Vector3d p1 = coordSystem.to_canonical(linkSegments.stepDownDestToSourceSegment->p1);
						Vector3d p2 = coordSystem.to_canonical(linkSegments.stepDownDestToSourceSegment->p2);
						navPolyK.add_link(NavLink_Ptr(new StepDownLink(mapIndexJ, edgeJ.navPolyIndex, p1, p2)));
					}
				}
				if(linkSegments.walkSegment)
				{
					// Add a walk link from j -> k, and one from k -> j.
					Vector3d p1 = coordSystem.to_canonical(linkSegments.walkSegment->p1);
					Vector3d p2 = coordSystem.to_canonical(linkSegments.walkSegment->p2);
					navPolyJ.add_link(NavLink_Ptr(new WalkLink(mapIndexJ, edgeK.navPolyIndex, p1, p2)));
					navPolyK.add_link(NavLink_Ptr(new WalkLink(mapIndexJ, edgeJ.navPolyIndex, p1, p2)));
				}
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
