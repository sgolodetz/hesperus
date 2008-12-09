/***
 * hesperus: Texture.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Texture.h"

namespace hesp {

//#################### HELPER CLASSES ####################
struct TextureDeleter
{
	void operator()(void *p)
	{
		GLuint *id = static_cast<GLuint*>(p);
		glDeleteTextures(1, id);
		delete p;
	}
};

//#################### CONSTRUCTORS ####################
/**
Constructs a new texture from an OpenGL texture ID.

@param id	The OpenGL texture ID
*/
Texture::Texture(GLuint id)
:	m_id(shared_ptr<GLuint>(new GLuint(id), TextureDeleter()))
{}

//#################### PUBLIC METHODS ####################
/**
Binds the texture to GL_TEXTURE_2D.
*/
void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, *m_id);
}

}
