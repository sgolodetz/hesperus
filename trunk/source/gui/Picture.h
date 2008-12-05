/***
 * hesperus: Picture.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GUI_PICTURE
#define H_HESP_GUI_PICTURE

#include "Component.h"

namespace hesp {

class Picture : public Component
{
	//#################### PUBLIC METHODS ####################
public:
	void render() const;
};

}

#endif
