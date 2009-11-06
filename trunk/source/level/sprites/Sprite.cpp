/***
 * hesperus: Sprite.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Sprite.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Sprite::Sprite(const Texture_Ptr& texture)
:	m_texture(texture)
{}

//#################### PUBLIC METHODS ####################
void Sprite::render(const Vector3d& position, double width, double height) const
{
	// NYI
	throw 23;
}

}
