/***
 * hesperus: UnlitLevelRenderer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UnlitLevelRenderer.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
UnlitLevelRenderer::UnlitLevelRenderer(const std::vector<TexturedPolygon_Ptr>& polygons)
:	m_polygons(polygons)
{}

//#################### PUBLIC METHODS ####################
void UnlitLevelRenderer::render(const std::vector<int>& indices) const
{
	// NYI
	throw 23;
}

}
