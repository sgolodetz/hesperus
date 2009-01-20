/***
 * hesperus: TextureFactory.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_TEXTUREFACTORY
#define H_HESP_TEXTUREFACTORY

#include <source/images/Image.h>
#include "Texture.h"

namespace hesp {

class TextureFactory
{
	//#################### PUBLIC METHODS ####################
public:
	static Texture_Ptr create_texture24(const Image24_CPtr& image);

	//#################### PRIVATE METHODS ####################
private:
	static void check_dimensions(int width, int height);
	static bool is_power_of_two(int n);
};

}

#endif
