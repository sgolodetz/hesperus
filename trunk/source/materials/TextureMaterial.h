/***
 * hesperus: TextureMaterial.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_TEXTUREMATERIAL
#define H_HESP_TEXTUREMATERIAL

#include <source/textures/Texture.h>
#include "Material.h"

namespace hesp {

class TextureMaterial : public Material
{
	//#################### PRIVATE VARIABLES ####################
private:
	Texture_Ptr m_texture;

	//#################### CONSTRUCTORS ####################
public:
	TextureMaterial(const Texture_Ptr& texture);

	//#################### PUBLIC METHODS ####################
public:
	void apply() const;
	bool uses_texcoords() const;
};

}

#endif
