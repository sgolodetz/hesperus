/***
 * hesperus: BrushExpander.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BrushExpander.h"

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
BrushExpander::ColPolyBrush_Ptr BrushExpander::expand_brush(const ColPolyBrush_Ptr& brush, const AABB3d& aabb)
{
	// Determine which planes need expanding (these are the face planes + any bevel planes).
	BrushPlaneSet_Ptr brushPlanes = determine_brush_planes(brush);

	// Expand the brush planes against the AABB.
	brushPlanes = expand_brush_planes(brushPlanes, aabb);

	std::vector<CollisionPolygon_Ptr> expandedFaces;

	// Generate the new collision faces from the expanded brush planes.
	for(BrushPlaneSet::const_iterator it=brushPlanes->begin(), iend=brushPlanes->end(); it!=iend; ++it)
	{
		// Build a large initial face on each brush plane.
		CollisionPolygon_Ptr face;
		if(it->auxData)
		{
			face = make_universe_polygon<CPAuxData>(it->plane, *(it->auxData));
		}
		else
		{
			// Construct an appropriate CPAuxData for any (axis-aligned) bevel plane we needed to add.

			// A bevel plane is only walkable if its normal is (0,0,1). Note that the bevel
			// planes are constructed manually in determine_brush_planes, so the normals are
			// exact (i.e. not subject to rounding errors).
			bool walkable = it->plane.normal().z == 1;

			face = make_universe_polygon<CPAuxData>(it->plane, CPAuxData(walkable));
		}

		// Clip it to the other planes.
		bool discard = false;
		for(BrushPlaneSet::const_iterator jt=brushPlanes->begin(), jend=brushPlanes->end(); jt!=jend; ++jt)
		{
			if(jt == it) continue;

			switch(classify_polygon_against_plane(*face, jt->plane))
			{
				case CP_BACK:
				{
					// The face is entirely behind this plane, so no clipping is needed.
					continue;
				}
				case CP_COPLANAR:
				{
					// The planes are unique, so this should never happen.
					throw Exception("BrushExpander: Unexpected duplicate plane");
				}
				case CP_FRONT:
				{
					// The face is entirely in front of this plane, so it's not part of the expanded brush.
					discard = true;
					break;
				}
				case CP_STRADDLE:
				{
					// The face straddles the plane, so split it and keep the bit behind it.
					SplitResults<Vector3d,CPAuxData> sr = split_polygon(*face, jt->plane);
					face = sr.back;
					break;
				}
			}

			if(discard) break;
		}

		// If it hasn't been clipped out of existence, add it to the expanded brush.
		if(!discard) expandedFaces.push_back(face);
	}

	// Construct a bounding box for the expanded brush.
	AABB3d expandedBounds = construct_bounding_box(expandedFaces);

	return ColPolyBrush_Ptr(new ColPolyBrush(expandedBounds, expandedFaces));
}

//#################### PRIVATE METHODS ####################
PlaneClassifier BrushExpander::classify_brush_against_plane(const ColPolyBrush_Ptr& brush, const Plane& plane)
{
	bool backFlag = false, frontFlag = false;

	const std::vector<CollisionPolygon_Ptr>& faces = brush->faces();
	int faceCount = static_cast<int>(faces.size());
	for(int i=0; i<faceCount; ++i)
	{
		switch(classify_polygon_against_plane(*faces[i], plane))
		{
		case CP_BACK:
			backFlag = true;
			break;
		case CP_COPLANAR:
			break;
		case CP_FRONT:
			frontFlag = true;
			break;
		case CP_STRADDLE:
			return CP_STRADDLE;
		}

		if(backFlag && frontFlag) return CP_STRADDLE;
	}

	if(backFlag) return CP_BACK;
	else if(frontFlag) return CP_FRONT;
	else return CP_COPLANAR;
}

BrushExpander::BrushPlaneSet_Ptr BrushExpander::determine_brush_planes(const ColPolyBrush_Ptr& brush)
{
	BrushPlaneSet_Ptr brushPlanes(new BrushPlaneSet);

	// Add the planes of all the brush faces.
	const std::vector<CollisionPolygon_Ptr>& faces = brush->faces();
	int faceCount = static_cast<int>(faces.size());
	for(int i=0; i<faceCount; ++i)
	{
		CPAuxData_Ptr auxData(new CPAuxData(faces[i]->auxiliary_data()));
		brushPlanes->insert(BrushPlane(make_plane(*faces[i]), auxData));
	}

	// Try and add bevel planes (any unnecessary ones will not be added to the set due to the unique plane predicate).

	// Add "axial planes going through an edge or corner of the brush that are not already part of the brush".
	const AABB3d& bounds = brush->bounds();
	brushPlanes->insert(BrushPlane(Plane(Vector3d(-1,0,0), -bounds.minimum().x)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,-1,0), -bounds.minimum().y)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,0,-1), -bounds.minimum().z)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(1,0,0), bounds.maximum().x)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,1,0), bounds.maximum().y)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,0,1), bounds.maximum().z)));

	// Along edges, add "all planes that are parallel to one of the coordinate axes and do not change
	// the shape of the brush".
	for(int i=0; i<faceCount; ++i)
	{
		int vertCount = faces[i]->vertex_count();
		for(int j=0; j<vertCount; ++j)
		{
			int k = (j+1)%vertCount;

			const Vector3d& p1 = faces[i]->vertex(j);
			const Vector3d& p2 = faces[i]->vertex(k);

			Plane_Ptr possiblePlanes[] =
			{
				make_bevel_plane(p1, p2, Vector3d(1,0,0)),
				make_bevel_plane(p1, p2, Vector3d(0,1,0)),
				make_bevel_plane(p1, p2, Vector3d(0,0,1))
			};

			for(int m=0; m<3; ++m)
			{
				if(possiblePlanes[m])
				{
					BrushPlane possiblePlane(*possiblePlanes[m]);
					BrushPlane flippedPossiblePlane(possiblePlanes[m]->flip());

					// If this plane (or its flipped equivalent) isn't currently in the brush plane set.
					if(	brushPlanes->find(possiblePlane) == brushPlanes->end() &&
						brushPlanes->find(flippedPossiblePlane) == brushPlanes->end())
					{
						// If the brush is entirely behind this plane, it's a valid
						// bevel plane, so add it. If the brush is entirely in front
						// of it, its inverse plane is a valid bevel plane, so add
						// that. Otherwise, carry on.
						switch(classify_brush_against_plane(brush, possiblePlane.plane))
						{
						case CP_BACK:
							brushPlanes->insert(possiblePlane);
							break;
						case CP_FRONT:
							brushPlanes->insert(flippedPossiblePlane);
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}

	return brushPlanes;
}

BrushExpander::BrushPlane
BrushExpander::expand_brush_plane(const BrushPlane& brushPlane, const AABB3d& aabb)
{
	const Vector3d& n = brushPlane.plane.normal();
	const double d = brushPlane.plane.distance_value();

	const double minX = aabb.minimum().x, minY = aabb.minimum().y, minZ = aabb.minimum().z;
	const double maxX = aabb.maximum().x, maxY = aabb.maximum().y, maxZ = aabb.maximum().z;

	Vector3d v;		// a vector from the AABB origin to an AABB vertex which would first hit the brush plane
	int xCode = n.x > 0 ? 4 : 0;
	int yCode = n.y > 0 ? 2 : 0;
	int zCode = n.z > 0 ? 1 : 0;
	int fullCode = xCode + yCode + zCode;

	switch(fullCode)
	{
		case 0: v = Vector3d(maxX, maxY, maxZ); break; // n: x-, y-, z-
		case 1: v = Vector3d(maxX, maxY, minZ); break; // n: x-, y-, z+
		case 2: v = Vector3d(maxX, minY, maxZ); break; // n: x-, y+, z-
		case 3: v = Vector3d(maxX, minY, minZ); break; // n: x-, y+, z+
		case 4: v = Vector3d(minX, maxY, maxZ); break; // n: x+, y-, z-
		case 5: v = Vector3d(minX, maxY, minZ); break; // n: x+, y-, z+
		case 6: v = Vector3d(minX, minY, maxZ); break; // n: x+, y+, z-
		default: v = Vector3d(minX, minY, minZ); break; // n: x+, y+, z+ (case 7)
	}

	// v . -n = |v| cos theta (see p.26 of J.M.P. van Waveren's thesis on the Q3 bot for a diagram)
	double expansionDistance = v.dot(-n);

	return BrushPlane(Plane(n, d+expansionDistance), brushPlane.auxData);
}

BrushExpander::BrushPlaneSet_Ptr
BrushExpander::expand_brush_planes(const BrushPlaneSet_Ptr& brushPlanes, const AABB3d& aabb)
{
	BrushPlaneSet_Ptr expandedBrushPlanes(new BrushPlaneSet);
	for(BrushPlaneSet::const_iterator it=brushPlanes->begin(), iend=brushPlanes->end(); it!=iend; ++it)
	{
		expandedBrushPlanes->insert(expand_brush_plane(*it, aabb));
	}
	return expandedBrushPlanes;
}

/**
Attempts to construct the plane in which v1, v2 and v1+axis all lie.

@param p1		A point
@param p2		Another point
@param axis		A vector (for our purposes, this will always be an axis vector like (0,0,1))
*/
Plane_Ptr BrushExpander::make_bevel_plane(const Vector3d& p1, const Vector3d& p2, const Vector3d& axis)
{
	Vector3d a = p2 - p1;
	Vector3d b = axis;
	Vector3d n = a.cross(b);

	if(n.length_squared() < EPSILON*EPSILON) return Plane_Ptr();
	else return Plane_Ptr(new Plane(n, p1));
}

}
