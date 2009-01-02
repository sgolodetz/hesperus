/***
 * hesperus: LevelRenderer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LevelRenderer.h"

#include <source/images/BitmapLoader.h>
#include <source/textures/TextureFactory.h>

namespace hesp {

//#################### PROTECTED METHODS ####################
void LevelRenderer::load_textures(const std::set<std::string>& textureNames)
{
	for(std::set<std::string>::const_iterator it=textureNames.begin(), iend=textureNames.end(); it!=iend; ++it)
	{
		std::string filename = "resources/textures/" + *it + ".bmp";
		Image24_Ptr image = BitmapLoader::load_image24(filename);
		m_textures.insert(std::make_pair(*it, TextureFactory::create_texture24(image)));
	}
}

}
