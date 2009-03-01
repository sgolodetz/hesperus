/***
 * hesperus: Image24Texture.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Image24Texture.h"

#include <source/ogl/WrappedGL.h>
#include <gl/glu.h>

#define GL_CLAMP_TO_EDGE 0x812F		// this wrapping mode is only defined in OpenGL 1.2, so it's not in the normal header (unfortunately)

namespace hesp {

//#################### CONSTRUCTORS ####################
Image24Texture::Image24Texture(const Image24_CPtr& image, bool clamp)
:	m_image(image), m_clamp(clamp)
{
	reload();
}

//#################### PROTECTED METHODS ####################
void Image24Texture::reload() const
{
	GLuint id;
	glGenTextures(1, &id);
	set_id(id);

	glBindTexture(GL_TEXTURE_2D, id);

	// Enable trilinear filtering for this texture when minifying.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Clamp the texture if necessary (useful for things like lightmaps, for example).
	if(m_clamp)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	int width = m_image->width();
	int height = m_image->height();
	int size = width*height;
	unsigned char *pixels = new unsigned char[size*3];
	for(int i=0; i<size; ++i)
	{
		const Image24::Pixel& p = (*m_image)(i);
		pixels[i*3]		= p.r();
		pixels[i*3+1]	= p.g();
		pixels[i*3+2]	= p.b();
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	delete[] pixels;
}

}
