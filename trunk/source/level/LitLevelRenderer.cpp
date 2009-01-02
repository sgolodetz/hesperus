/***
 * hesperus: LitLevelRenderer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LitLevelRenderer.h"

#include <source/textures/TextureFactory.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
LitLevelRenderer::LitLevelRenderer(const TexLitPolyVector& polygons, const std::vector<Image24_Ptr>& lightmaps)
:	m_polygons(polygons)
{
	assert(polygons.size() == lightmaps.size());

	// Determine the set of unique texture names.
	std::set<std::string> textureNames;
	int polyCount = static_cast<int>(polygons.size());
	for(int i=0; i<polyCount; ++i)
	{
		textureNames.insert(polygons[i]->auxiliary_data());
	}

	load_textures(textureNames);

	// Create the lightmaps.
	int lightmapCount = static_cast<int>(lightmaps.size());
	m_lightmaps.resize(lightmapCount);
	for(int i=0; i<lightmapCount; ++i)
	{
		m_lightmaps[i] = TextureFactory::create_texture24(lightmaps[i]);
	}
}

//#################### PUBLIC METHODS ####################
void LitLevelRenderer::render(const std::vector<int>& polyIndices) const
{
	// NYI
	//throw 23;
}

}
