/***
 * hesperus: LightmapGrid.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cmath>

#include <source/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs the lightmap grid for a polygon and calculates the lightmap coordinates for the polygon's vertices.

@param poly						The polygon
@param vertexLightmapCoords		Used to return the vertex lightmap coordinates to the caller
*/
template <typename Vert, typename AuxData>
LightmapGrid::LightmapGrid(const Polygon<Vert,AuxData>& poly, std::vector<TexCoords>& vertexLightmapCoords)
{
	AxisPlane bestAxisPlane = find_best_axis_plane(poly.normal());
	std::vector<Vector2d> projectedVertices = project_vertices_onto(poly, bestAxisPlane);
	make_planar_grid(projectedVertices, bestAxisPlane, vertexLightmapCoords);
	project_grid_onto_plane(make_plane(poly), bestAxisPlane);
}

/**
Project the polygon vertices onto a given axis.

@param poly			The polygon
@param axisPlane	The axis plane
@return				The projected vertices
*/
template <typename Vert, typename AuxData>
std::vector<Vector2d> LightmapGrid::project_vertices_onto(const Polygon<Vert,AuxData>& poly, AxisPlane axisPlane)
{
	int vertCount = poly.vertex_count();
	std::vector<Vector2d> projectedVertices;
	projectedVertices.reserve(vertCount);
	for(int i=0; i<vertCount; ++i)
	{
		projectedVertices.push_back(project_vertex_onto(Vector3d(poly.vertex(i)), axisPlane));
	}
	return projectedVertices;
}

}
