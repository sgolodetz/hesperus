/***
 * hesperus: LightmapGrid.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "LightmapGrid.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Calculates the lightmap of light shining on the polygon from a particular point source.
The idea is that we'll ultimately combine the lightmaps from each of the different
lights in the scene to make the final lightmap for the polygon.

@param light	The point light source
@param tree		The BSP tree for the level
@return			The lightmap shining on the polygon from the given light source
*/
Lightmap_Ptr LightmapGrid::lightmap_from_light(const Light& light, const BSPTree_Ptr& tree) const
{
	// NYI
	throw 23;
}

//#################### PRIVATE METHODS ####################
/**
Find the best axis plane onto which to project the polygon.

@param n	The polygon's normal
@return		The best axis plane
*/
LightmapGrid::AxisPlane LightmapGrid::find_best_axis_plane(const Vector3d& n)
{
	double absX = fabs(n.x), absY = fabs(n.y), absZ = fabs(n.z);

	AxisPlane bestAxisPlane = YZ_PLANE;
	double absLargest = absX;
	if(absY > absLargest)
	{
		bestAxisPlane = XZ_PLANE;
		absLargest = absY;
	}
	if(absZ > absLargest) bestAxisPlane = XY_PLANE;

	return bestAxisPlane;
}

/**
Makes an appropriately-sized grid around the projected vertices, and calculates
lightmap coordinates for each vertex.

@param projectedVertices		The projection of the original polygon's vertices onto the best axis plane
@param axisPlane				The axis plane
@param vertexLightmapCoords		Used to return lightmap coordinates for each vertex to the caller
*/
void LightmapGrid::make_planar_grid(const std::vector<Vector2d>& projectedVertices, AxisPlane axisPlane, std::vector<TexCoords>& vertexLightmapCoords)
{
	double minX, minY, maxX, maxY;
	minX = minY = INT_MAX;
	maxX = maxY = INT_MIN;



	// NYI
	throw 23;
}

/**
Converts a vertex lying in one of the axis planes back into its 3D representation.

@param v			The planar vertex
@param axisPlane	The axis plane
@return				The 3D representation of the vertex
*/
Vector3d LightmapGrid::planar_to_real(const Vector2d& v, AxisPlane axisPlane)
{
	switch(axisPlane)
	{
		case YZ_PLANE:
			return Vector3d(0, v.x, v.y);
		case XZ_PLANE:
			return Vector3d(v.x, 0, v.y);
		default:	// case XY_PLANE
			return Vector3d(v.x, v.y, 0);
	}
}

/**
Project the vertex onto a given axis plane.

@param v		The vertex
@param axis		The axis plane
@return			The projected vertex
*/
Vector2d LightmapGrid::project_vertex_onto(const Vector3d& v, AxisPlane axisPlane)
{
	switch(axisPlane)
	{
		case YZ_PLANE:
		{
			// Project onto x = 0, i.e. the y-z plane.
			return Vector2d(v.y, v.z);
		}
		case XZ_PLANE:
		{
			// Project onto y = 0, i.e. the x-z plane.
			return Vector2d(v.x, v.z);
		}
		default:	// case XY_PLANE
		{
			// Project onto z = 0, i.e. the x-y plane.
			return Vector2d(v.x, v.y);
		}
	}
}

}
