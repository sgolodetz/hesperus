/***
 * hesperus: LightmapGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LIGHTING_LIGHTMAPGENERATOR
#define H_HESP_LEVEL_LIGHTING_LIGHTMAPGENERATOR

#include <vector>

#include <source/level/bsp/BSPTree.h>
#include <source/level/vis/VisTable.h>
#include <source/util/PolygonTypes.h>
#include "Light.h"
#include "Lightmap.h"
#include "LightmapGrid.h"

namespace hesp {

class LightmapGenerator
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<LightmapGrid_Ptr> LightmapGridVector;
	typedef std::vector<Lightmap_Ptr> LightmapVector;
	typedef shared_ptr<LightmapVector> LightmapVector_Ptr;
	typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;
	typedef std::vector<TexturedLitPolygon_Ptr> TexLitPolyVector;
	typedef shared_ptr<TexLitPolyVector> TexLitPolyVector_Ptr;

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	TexPolyVector m_inputPolygons;
	std::vector<Light> m_lights;
	BSPTree_Ptr m_tree;
	LeafVisTable_Ptr m_leafVis;

	// Intermediate data
	LightmapGridVector m_grids;

	// Output data
	TexLitPolyVector_Ptr m_outputPolygons;
	LightmapVector_Ptr m_lightmaps;

	//#################### CONSTRUCTORS ####################
public:
	LightmapGenerator(const TexPolyVector& inputPolygons, const std::vector<Light>& lights, const BSPTree_Ptr& tree, const LeafVisTable_Ptr& leafVis);

	//#################### PUBLIC METHODS ####################
public:
	void generate_lightmaps();
	LightmapVector_Ptr lightmaps() const;
	TexLitPolyVector_Ptr lit_polygons() const;

	//#################### PRIVATE METHODS ####################
private:
	void clean_intermediate();
	void construct_grid(int n);
	void construct_grids();
	void process_light(int n);
	void process_lights();
};

}

#endif
