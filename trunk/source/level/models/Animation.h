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
	double m_length;
	std::vector<Keyframe_Ptr> m_keyframes;

	//#################### CONSTRUCTORS ####################
public:
	Animation(double length, const std::vector<Keyframe_Ptr>& keyframes);

	//#################### PUBLIC METHODS ####################
public:
	const Keyframe& keyframes(int i) const;
	double length() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Animation> Animation_Ptr;
typedef shared_ptr<const Animation> Animation_CPtr;

}

#endif
