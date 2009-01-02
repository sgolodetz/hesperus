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
		if(glIsTexture(*id)) glDeleteTextures(1, id);
		delete p;
	}
};

//#################### CONSTRUCTORS ####################
/**
Constructs an empty new texture (the subclass constructor will initialise it).
*/
Texture::Texture() {}

//#################### DESTRUCTOR ####################
Texture::~Texture() {}

//#################### PUBLIC METHODS ####################
/**
Binds the texture to GL_TEXTURE_2D (reloads it first if necessary).
*/
void Texture::bind() const
{
	if(!glIsTexture(*m_id)) reload();
	glBindTexture(GL_TEXTURE_2D, *m_id);
}

//#################### PROTECTED METHODS ####################
/**
Sets the texture ID (for use by subclasses during reloading).

@param id	The new texture ID
*/
void Texture::set_id(GLuint id) const
{
	m_id.reset(new GLuint(id), TextureDeleter());
}

}
