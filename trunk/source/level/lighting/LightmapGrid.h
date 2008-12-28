/***
 * hesperus: LightmapGrid.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LIGHTING_LIGHTMAPGRID
#define H_HESP_LEVEL_LIGHTING_LIGHTMAPGRID

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/bsp/BSPTree.h>
#include <source/math/geom/Polygon.h>
#include <source/math/vectors/TexCoords.h>
#include "Light.h"
#include "Lightmap.h"

namespace hesp {

class LightmapGrid
{
	//#################### NESTED CLASSES ####################
private:
	struct GridPoint
	{
		Vector3d position;		// where does the grid point lie in world space?
		bool withinPolygon;		// is it within the polygon being lightmapped? (if not, we can skip some work when constructing a lightmap)
	};

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<std::vector<GridPoint> > m_grid;

	//#################### STATIC FACTORY METHODS ####################
public:
	template <typename Vert, typename AuxData>
	static LightmapGrid construct(const Polygon<Vert,AuxData>& poly, std::vector<TexCoords>& vertexLightmapCoords);

	//#################### PUBLIC METHODS ####################
public:
	Lightmap_Ptr lightmap_from_light(const Light& light, const BSPTree_Ptr& tree) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<LightmapGrid> LightmapGrid_Ptr;
typedef shared_ptr<const LightmapGrid> LightmapGrid_CPtr;

}

#include "LightmapGrid.tpp"

#endif
