/***
 * hesperus: LitLevelRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LITLEVELRENDERER
#define H_HESP_LEVEL_LITLEVELRENDERER

#include <source/images/Image.h>
#include <source/util/PolygonTypes.h>
#include "LevelRenderer.h"

namespace hesp {

class LitLevelRenderer : public LevelRenderer
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<TexturedLitPolygon_Ptr> TexLitPolyVector;

	//#################### PRIVATE VARIABLES ####################
private:
	TexLitPolyVector m_polygons;
	// TODO

	//#################### CONSTRUCTORS ####################
public:
	LitLevelRenderer(const TexLitPolyVector& polygons, const std::vector<Image24_Ptr>& lightmaps);

	//#################### PUBLIC METHODS ####################
public:
	void render(const std::vector<int>& indices) const;
};

}

#endif
