/***
 * hesperus: Animation.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ANIMATION
#define H_HESP_ANIMATION

#include <vector>

#include "Keyframe.h"

namespace hesp {

class Animation
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Keyframe> m_keyframes;

	// TODO
};

}

#endif
