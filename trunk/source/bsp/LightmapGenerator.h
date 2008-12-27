/***
 * hesperus: LightmapGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_LIGHTMAPGENERATOR
#define H_HESP_BSP_LIGHTMAPGENERATOR

#include <vector>

#include <source/util/PolygonTypes.h>
#include "BSPTree.h"
#include "Light.h"
#include "VisTable.h"

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
