/***
 * hesperus: Sprite.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Sprite.h"

#include <source/ogl/WrappedGL.h>
#include <source/textures/Texture.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Sprite::Sprite(const Texture_Ptr& texture)
:	m_texture(texture)
{}

//#################### PUBLIC METHODS ####################
void Sprite::render(const Vector3d& position, double width, double height) const
{
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	m_texture->bind();

	// TODO

	glPopAttrib();
}

}
