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
LightmapGenerator::LightmapVector_Ptr LightmapGenerator::lightmaps() const
{
	return m_lightmaps;
}

LightmapGenerator::TexLitPolyVector_Ptr LightmapGenerator::lit_polygons() const
{
	return m_outputPolygons;
}

void LightmapGenerator::run()
{
	// NYI
	throw 23;
}

//#################### PRIVATE METHODS ####################
void LightmapGenerator::clean_intermediate()
{
	// NYI
	throw 23;
}

void LightmapGenerator::construct_grids()
{
	// NYI
	throw 23;
}

void LightmapGenerator::process_light(int i)
{
	// NYI
	throw 23;
}

void LightmapGenerator::process_lights()
{
	int lightCount = static_cast<int>(m_lights.size());
	for(int i=0; i<lightCount; ++i)
	{
		process_light(i);
	}
}

}
