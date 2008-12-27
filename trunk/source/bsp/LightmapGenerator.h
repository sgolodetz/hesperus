/***
 * hesperus: LightmapGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_LIGHTMAPGENERATOR
#define H_HESP_BSP_LIGHTMAPGENERATOR

#include <vector>

#include <source/util/PolygonTypes.h>

namespace hesp {

class LightmapGenerator
{
	//#################### TYPEDEFS ####################
private:
	// TODO

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<TexturedPolygon_Ptr> m_inputPolygons;

	//#################### CONSTRUCTORS ####################
public:
	LightmapGenerator(const std::vector<TexturedPolygon_Ptr>& inputPolygons, const BSPTree_Ptr& tree);

	//#################### PUBLIC METHODS ####################
public:
	// TODO
};

}

#endif
