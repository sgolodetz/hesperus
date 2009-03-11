/***
 * hesperus: BasicMaterial.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BasicMaterial.h"

#include <source/ogl/WrappedGL.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
BasicMaterial::BasicMaterial(const Colour3d& ambient, const Colour3d& diffuse, const Colour3d& specular, double specularExponent, const Colour3d& emissive)
:	m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_specularExponent(specularExponent), m_emissive(emissive)
{}

//#################### PUBLIC METHODS ####################
const Colour3d& BasicMaterial::ambient() const	{ return m_ambient; }

void BasicMaterial::apply() const
{
	// FIXME: Use the material properly.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
}

const Colour3d& BasicMaterial::diffuse() const	{ return m_diffuse; }
const Colour3d& BasicMaterial::emissive() const	{ return m_emissive; }
const Colour3d& BasicMaterial::specular() const	{ return m_specular; }
double BasicMaterial::specular_exponent() const	{ return m_specularExponent; }

bool BasicMaterial::uses_texcoords() const
{
	return false;
}

}
