/***
 * hesperus: PNGLoader.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PNGLOADER
#define H_HESP_PNGLOADER

#include <istream>
#include <string>

#include "Image.h"

namespace hesp {

/**
This struct provides loading functions for PNGs of various bit depths (e.g. 24-bit).
*/
struct PNGLoader
{
	//#################### PUBLIC METHODS ####################
	static Image24_Ptr load_image24(const std::string& filename);
};

}

#endif
