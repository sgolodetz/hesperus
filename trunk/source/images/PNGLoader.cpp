/***
 * hesperus: PNGLoader.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PNGLoader.h"

#include <fstream>

#include <lodepng.h>

#include <source/exceptions/FileNotFoundException.h>
#include "SimpleImage.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads a 24-bit PNG from a file.

@param filename		The name of the file in which the PNG is stored
@return				An Image24_Ptr holding the representation of the image
*/
Image24_Ptr PNGLoader::load_image24(const std::string& filename)
{
	// Load the raw data.
	std::vector<unsigned char> buffer;
	LodePNG::loadFile(buffer, filename);
	if(buffer.empty()) throw FileNotFoundException(filename);

	// Decode the PNG.
	std::vector<unsigned char> data;
	LodePNG::Decoder decoder;
	decoder.decode(data, &buffer[0], buffer.size());
	if(decoder.hasError()) throw Exception("An error occurred whilst trying to decode the PNG in " + filename);

	// Construct the 24-bit image.
	int width = decoder.getWidth();
	int height = decoder.getHeight();

	const int pixelCount = width*height;
	Pixel24 *const pixels = new Pixel24[pixelCount];

	unsigned char *src = &data[0];
	Pixel24 *dest = pixels;
	for(int i=0; i<pixelCount; ++i)
	{
		*dest = Pixel24(*src, *(src+1), *(src+2));
		++dest;
		src += 4;	// note that we skip the alpha component each time (that's why it's += 4 not += 3)
	}

	return Image24_Ptr(new SimpleImage24(pixels, width, height));
}

}
