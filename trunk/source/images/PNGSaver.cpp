/***
 * hesperus: PNGSaver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PNGSaver.h"

#include <lodepng.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Saves a 24-bit PNG to a file.

@param filename		The name of the file to which to save the image
@param image		An Image24_Ptr holding the representation of the image
*/
void PNGSaver::save_image24(const std::string& filename, const Image24_Ptr& image)
{
	int width = image->width();
	int height = image->height();
	const int pixelCount = width*height;

	// Construct the image data array.
	std::vector<unsigned char> data(pixelCount*4);
	unsigned char *p = &data[0];
	for(int y=0; y<height; ++y)
		for(int x=0; x<width; ++x)
		{
			Pixel24 pixel = (*image)(x,y);
			*p		= pixel.r();
			*(p+1)	= pixel.g();
			*(p+2)	= pixel.b();
			*(p+3)	= 255;
			p += 4;
		}

	// Encode and save the PNG.
	LodePNG::encode(filename, data, width, height);
}

}
