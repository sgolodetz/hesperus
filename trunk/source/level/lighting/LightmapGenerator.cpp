/***
 * hesperus: LightmapGenerator.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "LightmapGenerator.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
LightmapGenerator::LightmapGenerator(const TexPolyVector_Ptr& inputPolygons, const std::vector<Light>& lights, const BSPTree_Ptr& tree, const LeafVisTable_Ptr& leafVis)
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
Constructs the lightmap grid for polygon i.

@param i	The index of the polygon for which to build a lightmap grid
*/
void LightmapGenerator::construct_grid(int i)
{
	// NYI
	throw 23;
}

/**
Constructs the lightmap grids for each polygon.
*/
void LightmapGenerator::construct_grids()
{
	int polyCount = static_cast<int>(m_inputPolygons->size());
	m_outputPolygons.reset(new TexLitPolyVector(polyCount));

	for(int i=0; i<polyCount; ++i)
	{
		construct_grid(i);
	}
}

/**
Processes light i: this involves updating the lightmaps for all the polygons that
the light can see.

@param i	The index of the light to be processed
*/
void LightmapGenerator::process_light(int i)
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
