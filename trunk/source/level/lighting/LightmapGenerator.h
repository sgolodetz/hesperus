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

namespace hesp {

class LightmapGenerator
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<TexturedPolygon_Ptr> TPolyVector;
	typedef std::vector<TexturedLitPolygon_Ptr> TLPolyVector;
	typedef shared_ptr<TLPolyVector> TLPolyVector_Ptr;

	//#################### PRIVATE VARIABLES ####################
private:
	TPolyVector m_inputPolygons;

	//#################### CONSTRUCTORS ####################
public:
	LightmapGenerator(const TPolyVector& inputPolygons, const std::vector<Light>& lights, const BSPTree_Ptr& tree, const LeafVisTable_Ptr& leafVis);

	//#################### PUBLIC METHODS ####################
public:
	// TODO: lightmaps()
	TLPolyVector_Ptr lit_polygons() const;
};

}

#endif
