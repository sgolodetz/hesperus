/***
 * hesperus: PNGSaver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PNGSAVER
#define H_HESP_PNGSAVER

#include <iosfwd>
#include <string>

#include "Image.h"

namespace hesp {

/**
This struct provides saving functions for PNGs of various bit depths (e.g. 24-bit).
*/
struct PNGSaver
{
	//#################### PUBLIC METHODS ####################
	static void save_image24(const std::string& filename, const Image24_Ptr& image);
};

}

#endif
