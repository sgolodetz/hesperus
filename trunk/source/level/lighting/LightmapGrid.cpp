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
@return		The best axis
*/
LightmapGrid::Axis LightmapGrid::find_best_axis(const Vector3d& n)
{
	double absX = fabs(n.x), absY = fabs(n.y), absZ = fabs(n.z);

	Axis bestAxis = X_AXIS;
	double absLargest = absX;
	if(absY > absLargest)
	{
		bestAxis = Y_AXIS;
		absLargest = absY;
	}
	if(absZ > absLargest) bestAxis = Z_AXIS;

	return bestAxis;
}

/**
Makes an appropriately-sized grid around the projected vertices, and calculates
lightmap coordinates for each vertex.

@param projectedVertices		The projection of the original polygon's vertices onto the best axis plane
@param vertexLightmapCoords		Used to return lightmap coordinates for each vertex to the caller
*/
void LightmapGrid::make_planar_grid(const std::vector<Vector3d>& projectedVertices, std::vector<TexCoords>& vertexLightmapCoords)
{
	// NYI
	throw 23;
}

/**
Project the vertex onto a given axis.

@param v		The vertex
@param axis		The axis
@return			The projected vertex
*/
Vector3d LightmapGrid::project_vertex_onto(const Vector3d& v, Axis axis)
{
	switch(axis)
	{
		case X_AXIS:
		{
			// Project onto x = 0, i.e. the y-z plane.
			return Vector3d(0, v.y, v.z);
		}
		case Y_AXIS:
		{
			// Project onto y = 0, i.e. the x-z plane.
			return Vector3d(v.x, 0, v.z);
		}
		default:	// case Z_AXIS
		{
			// Project onto z = 0, i.e. the x-y plane.
			return Vector3d(v.x, v.y, 0);
		}
	}
}

}
