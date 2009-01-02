/***
 * hesperus: UnlitLevelRenderer.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_UNLITLEVELRENDERER
#define H_HESP_LEVEL_UNLITLEVELRENDERER

#include <map>
#include <string>

#include <source/textures/Texture.h>
#include <source/util/PolygonTypes.h>
#include "LevelRenderer.h"

namespace hesp {

class UnlitLevelRenderer : public LevelRenderer
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;

	//#################### PRIVATE VARIABLES ####################
private:
	TexPolyVector m_polygons;
	std::map<std::string,Texture_Ptr> m_textures;

	//#################### CONSTRUCTORS ####################
public:
	UnlitLevelRenderer(const TexPolyVector& polygons);

	//#################### PUBLIC METHODS ####################
public:
	void render(const std::vector<int>& polyIndices) const;

	//#################### PRIVATE METHODS ####################
private:
	void render_simple(const std::vector<int>& polyIndices) const;
	void render_proper(const std::vector<int>& polyIndices) const;
};

}

#endif
