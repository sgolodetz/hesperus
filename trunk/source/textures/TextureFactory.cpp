/***
 * hesperus: TextureFactory.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "TextureFactory.h"

#include <gl/glu.h>

#include <source/exceptions/InvalidParameterException.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Creates a texture from a 24-bit image.

@param image	The image from which to create the texture
@return			The created texture
*/
Texture_Ptr TextureFactory::create_texture24(const Image24_CPtr& image)
{
	int width = image->width(), height = image->height();
	check_dimensions(width, height);

	GLuint id;
	glGenTextures(1, &id);

	glBindTexture(GL_TEXTURE_2D, id);

	// Enable trilinear filtering for this texture when minifying.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int size = width*height;
	unsigned char *pixels = new unsigned char[size*3];
	for(int i=0; i<size; ++i)
	{
		const Image24::Pixel& p = (*image)(i);
		pixels[i*3]		= p.r();
		pixels[i*3+1]	= p.g();
		pixels[i*3+2]	= p.b();
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	delete[] pixels;

	return Texture_Ptr(new Texture(id));
}

//#################### PRIVATE METHODS ####################
/**
Checks that the proposed dimensions of the texture are valid.

@param width						The proposed width of the texture
@param height						The proposed height of the texture
@throws InvalidParameterException	If the proposed dimensions are invalid
*/
void TextureFactory::check_dimensions(int width, int height)
{
	if(width < 2 || width > 512 || height < 2 || height > 512) throw InvalidParameterException("Image dimensions out of range");
	if(!is_power_of_two(width) || !is_power_of_two(height)) throw InvalidParameterException("Image dimensions are not powers of two");
}

/**
Returns whether or not n is a power of two.
*/
bool TextureFactory::is_power_of_two(int n)
{
	if(n < 1) return false;

	while(n != 1)
	{
		if(n % 2 != 0) return false;
		n = n >> 1;
	}

	return true;
}

}
