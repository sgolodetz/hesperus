/***
 * hesperus: LightmapGrid.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LIGHTING_LIGHTMAPGRID
#define H_HESP_LEVEL_LIGHTING_LIGHTMAPGRID

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/Polygon.h>
#include <source/math/vectors/TexCoords.h>

namespace hesp {

class LightmapGrid
{
	//#################### PRIVATE VARIABLES ####################
private:
	// TODO

	//#################### STATIC FACTORY METHODS ####################
public:
	template <typename Vert, typename AuxData>
	static LightmapGrid construct(const Polygon<Vert,AuxData>& poly, std::vector<TexCoords>& vertexLightmapCoords);

	//#################### PUBLIC METHODS ####################
public:
	// TODO
};

//#################### TYPEDEFS ####################
typedef shared_ptr<LightmapGrid> LightmapGrid_Ptr;
typedef shared_ptr<const LightmapGrid> LightmapGrid_CPtr;

}

#include "LightmapGrid.tpp"

#endif
