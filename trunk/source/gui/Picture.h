/***
 * hesperus: Picture.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_PICTURE
#define H_HESP_PICTURE

#include <string>

#include <source/textures/Texture.h>
#include "GUIComponent.h"

namespace hesp {

class Picture : public GUIComponent
{
	//#################### PRIVATE VARIABLES ####################
private:
	Texture_Ptr m_texture;

	//#################### CONSTRUCTORS ####################
public:
	explicit Picture(const std::string& filename);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;
};

}

#endif
