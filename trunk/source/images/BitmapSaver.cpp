/***
 * hesperus: BitmapSaver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BitmapSaver.h"

#include <fstream>
#include <vector>

#ifdef _WIN32
	// Provides BITMAPFILEHEADER, BITMAPINFOHEADER, etc.
	#include <windows.h>
#else
	#error Need to define the relevant bitmap structs for non-Windows platforms.
#endif

#include <source/exceptions/FileNotFoundException.h>
#include <source/images/SimpleImage.h>
using namespace hesp;

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Saves a 24-bit bitmap to a file.

@param filename		The name of the file to which to save the image
@param image		An Image24_Ptr holding the representation of the image
*/
void BitmapSaver::save_image24(const std::string& filename, const Image24_Ptr& image)
{
	// NYI
	throw 23;
}

/**
Saves a 24-bit bitmap to a std::ostream.

@param os		The std::ostream to which to save the image
@param image	An Image24_Ptr holding the representation of the image
*/
void BitmapSaver::save_image24(std::ostream& os, const Image24_Ptr& image)
{
	// NYI
	throw 23;
}

}
