/***
 * hesperus: Material.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATERIAL
#define H_HESP_MATERIAL

#include <source/colours/Colour3d.h>

namespace hesp {

class Material
{
	//#################### PRIVATE VARIABLES ####################
private:
	Colour3d m_ambient;
	Colour3d m_diffuse;
	Colour3d m_emissive;
	Colour3d m_specular;

	//#################### CONSTRUCTORS ####################
public:
	Material(const Colour3d& ambient, const Colour3d& diffuse, const Colour3d& specular, const Colour3d& emissive);

	//#################### PUBLIC METHODS ####################
public:
	const Colour3d& ambient() const;
	const Colour3d& diffuse() const;
	const Colour3d& emissive() const;
	const Colour3d& specular() const;
};

}

#endif
