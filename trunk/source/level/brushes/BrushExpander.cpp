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

		// TODO: Clip it to the other planes.
		// NYI
		throw 23;
	}

	// NYI
	throw 23;
}

//#################### PRIVATE METHODS ####################
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
	const AABB3d& bounds = brush->bounds();
	brushPlanes->insert(BrushPlane(Plane(Vector3d(-1,0,0), -bounds.minimum().x)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,-1,0), -bounds.minimum().y)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,0,-1), -bounds.minimum().z)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(1,0,0), bounds.maximum().x)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,1,0), bounds.maximum().y)));
	brushPlanes->insert(BrushPlane(Plane(Vector3d(0,0,1), bounds.maximum().z)));

	return brushPlanes;
}

BrushExpander::BrushPlane
BrushExpander::expand_brush_plane(const BrushPlane& brushPlane, const AABB3d& aabb)
{
	// NYI
	throw 23;
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

}
