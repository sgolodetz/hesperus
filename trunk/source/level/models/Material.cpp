/***
 * hesperus: Material.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Material.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Material::Material(const Colour3d& ambient, const Colour3d& diffuse, const Colour3d& specular, const Colour3d& emissive)
:	m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_emissive(emissive)
{}

//#################### PUBLIC METHODS ####################
const Colour3d& Material::ambient() const	{ return m_ambient; }
const Colour3d& Material::diffuse() const	{ return m_diffuse; }
const Colour3d& Material::emissive() const	{ return m_emissive; }
const Colour3d& Material::specular() const	{ return m_specular; }

}
