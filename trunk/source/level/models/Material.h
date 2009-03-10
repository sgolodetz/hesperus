/***
 * hesperus: Material.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATERIAL
#define H_HESP_MATERIAL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

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
	double m_specularExponent;

	//#################### CONSTRUCTORS ####################
public:
	Material(const Colour3d& ambient, const Colour3d& diffuse, const Colour3d& specular, double specularExponent,
			 const Colour3d& emissive);

	//#################### PUBLIC METHODS ####################
public:
	const Colour3d& ambient() const;
	const Colour3d& diffuse() const;
	const Colour3d& emissive() const;
	const Colour3d& specular() const;
	double specular_exponent() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Material> Material_Ptr;
typedef shared_ptr<const Material> Material_CPtr;

}

#endif
