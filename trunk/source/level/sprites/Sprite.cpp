/***
 * hesperus: Sprite.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Sprite.h"

#include <source/math/Constants.h>
#include <source/ogl/WrappedGL.h>
#include <source/textures/Texture.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Sprite::Sprite(const Texture_Ptr& texture)
:	m_texture(texture)
{}

//#################### PUBLIC METHODS ####################
void Sprite::render(const Vector3d& spritePos, const Vector3d& cameraPos, double width, double height) const
{
	// Calculate the sprite's local NUV coordinate system.
	Vector3d spriteN = cameraPos - spritePos;
	if(spriteN.length_squared() < EPSILON*EPSILON) return;	// too close to the sprite to render it
	spriteN.normalize();

	Vector3d spriteU = Vector3d(0,0,1).cross(spriteN);
	if(spriteU.length_squared() < EPSILON*EPSILON) spriteU = Vector3d(1,0,0).cross(spriteN);

	Vector3d spriteV = spriteN.cross(spriteU);

	// Render the sprite.
	glPushAttrib(GL_ENABLE_BIT);

	glEnable(GL_TEXTURE_2D);
	m_texture->bind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Vector3d tl = spritePos - spriteU + spriteV;
	Vector3d tr = spritePos + spriteU + spriteV;
	Vector3d bl = spritePos - spriteU - spriteV;
	Vector3d br = spritePos + spriteU - spriteV;

	glColor3d(1,1,1);
	glBegin(GL_QUADS);
		glTexCoord2d(0,0);	glVertex3d(tl.x, tl.y, tl.z);
		glTexCoord2d(0,1);	glVertex3d(bl.x, bl.y, bl.z);
		glTexCoord2d(1,1);	glVertex3d(br.x, br.y, br.z);
		glTexCoord2d(1,0);	glVertex3d(tr.x, tr.y, tr.z);
	glEnd();

	glPopAttrib();
}

}
