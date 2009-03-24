/***
 * hesperus: ImageLoader.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ImageLoader.h"

#include <source/exceptions/Exception.h>
#include "BitmapLoader.h"
#include "PNGLoader.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
Image24_Ptr ImageLoader::load_image24(const std::string& filename)
{
	std::string::size_type i = filename.find_last_of('.');
	if(i == std::string::npos) throw Exception("Could not deduce image file type from non-existent file extension");
	std::string extension = filename.substr(i+1);

	if(extension == "bmp") return BitmapLoader::load_image24(filename);
	else if(extension == "png") return PNGLoader::load_image24(filename);
	else throw Exception("Unknown image file extension: " + extension);
}

}
