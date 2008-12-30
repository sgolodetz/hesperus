/***
 * hesperus: LightmapGenerator.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "LightmapGenerator.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
LightmapGenerator::LightmapGenerator(const TexPolyVector& inputPolygons, const std::vector<Light>& lights, const BSPTree_Ptr& tree, const LeafVisTable_Ptr& leafVis)
:	m_inputPolygons(inputPolygons), m_lights(lights), m_tree(tree), m_leafVis(leafVis)
{}

//#################### PUBLIC METHODS ####################
/**
Generates a lit polygon and a lightmap for each polygon in the input array.
*/
void LightmapGenerator::generate_lightmaps()
{
	if(!m_lightmaps)
	{
		construct_grids();
		construct_ambient_lightmaps();
		process_lights();
		clean_intermediate();
	}
}

/**
Returns the array of calculated lightmaps (the generator should have been run first).

@return	As stated
*/
LightmapGenerator::LightmapVector_Ptr LightmapGenerator::lightmaps() const
{
	return m_lightmaps;
}

/**
Returns the array of lit polygons (the generator should have been run first).

@return	As stated
*/
LightmapGenerator::TexLitPolyVector_Ptr LightmapGenerator::lit_polygons() const
{
	return m_outputPolygons;
}

//#################### PRIVATE METHODS ####################
/**
Cleans up the intermediate data structures used during lightmap generation.
*/
void LightmapGenerator::clean_intermediate()
{
	m_grids.swap(LightmapGridVector());
}

/**
Constructs the initial lightmaps based on the ambient light in the scene.
*/
void LightmapGenerator::construct_ambient_lightmaps()
{
	int polyCount = static_cast<int>(m_inputPolygons.size());
	m_lightmaps.reset(new LightmapVector(polyCount));

	for(int i=0; i<polyCount; ++i)
	{
		// TODO
	}

	// NYI
	throw 23;
}

/**
Constructs the lightmap grid for polygon n.

@param n	The index of the polygon for which to build a lightmap grid
*/
void LightmapGenerator::construct_grid(int n)
{
	std::vector<TexCoords> vertexLightmapCoords;
	m_grids[n].reset(new LightmapGrid(*m_inputPolygons[n], vertexLightmapCoords));

	int vertCount = m_inputPolygons[n]->vertex_count();
	std::vector<TexturedLitVector3d> vertices;
	vertices.reserve(vertCount);
	for(int i=0; i<vertCount; ++i)
	{
		const TexturedVector3d& v = m_inputPolygons[n]->vertex(i);
		vertices.push_back(TexturedLitVector3d(v.x, v.y, v.z, v.u, v.v, vertexLightmapCoords[i].u, vertexLightmapCoords[i].v));
	}

	// Note:	We copy the texture across from the input polygon. We don't need to store a lightmap index
	//			because lightmaps will be loaded into an array corresponding to the polygon array anyway
	//			(there's one lightmap per polygon).
	(*m_outputPolygons)[n].reset(new TexturedLitPolygon(vertices, m_inputPolygons[n]->auxiliary_data()));
}

/**
Constructs the lightmap grids for each polygon.
*/
void LightmapGenerator::construct_grids()
{
	int polyCount = static_cast<int>(m_inputPolygons.size());
	m_grids.resize(polyCount);
	m_outputPolygons.reset(new TexLitPolyVector(polyCount));

	for(int i=0; i<polyCount; ++i)
	{
		construct_grid(i);
	}
}

/**
Processes light n: this involves updating the lightmaps for all the polygons that
the light can see.

@param n	The index of the light to be processed
*/
void LightmapGenerator::process_light(int n)
{
	// NYI
	throw 23;
}

/**
Processes all the lights in the level.
*/
void LightmapGenerator::process_lights()
{
	int lightCount = static_cast<int>(m_lights.size());
	for(int i=0; i<lightCount; ++i)
	{
		process_light(i);
	}
}

}
