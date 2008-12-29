/***
 * hesperus: LightmapGrid.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cmath>

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
	Axis bestAxis = find_best_axis(poly.normal());
	std::vector<Vector2d> projectedVertices = project_vertices_onto(poly, bestAxis);
	make_planar_grid(projectedVertices, bestAxis, vertexLightmapCoords);
	project_grid_onto_polygon(poly);
}

template <typename Vert, typename AuxData>
void LightmapGrid::project_grid_onto_polygon(const Polygon<Vert,AuxData>& poly)
{
	// NYI
	throw 23;
}

/**
Project the polygon vertices onto a given axis.

@param poly		The polygon
@param axis		The axis
@return			The projected vertices
*/
template <typename Vert, typename AuxData>
std::vector<Vector2d> LightmapGrid::project_vertices_onto(const Polygon<Vert,AuxData>& poly, Axis axis)
{
	int vertCount = poly.vertex_count();
	std::vector<Vector2d> projectedVertices;
	projectedVertices.reserve(vertCount);
	for(int i=0; i<vertCount; ++i)
	{
		projectedVertices.push_back(project_vertex_onto(Vector3d(poly.vertex(i)), axis));
	}
	return projectedVertices;
}

}
