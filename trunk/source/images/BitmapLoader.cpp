/***
 * hesperus: BitmapLoader.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BitmapLoader.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads a 24-bit bitmap from a file.

@param filename		The name of the file in which the bitmap is stored
@return				An Image24_Ptr holding the representation of the image
*/
Image24_Ptr BitmapLoader::load_image24(const std::string& filename)
{
	// NYI
	throw 23;
}

/**
Loads a 24-bit bitmap from a std::istream.

@param is	The std::istream from which to load the bitmap
@return		An Image24_Ptr holding the representation of the image
*/
Image24_Ptr BitmapLoader::load_image24(std::istream& is)
{
	// NYI
	throw 23;
}

}
