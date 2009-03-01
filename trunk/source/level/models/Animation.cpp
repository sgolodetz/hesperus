/***
 * hesperus: Animation.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Animation.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Animation::Animation(double length, const std::vector<Keyframe_Ptr>& keyframes)
:	m_length(length), m_keyframes(keyframes)
{}

//#################### PUBLIC METHODS ####################
const Keyframe& Animation::keyframes(int i) const
{
	if(0 <= i && i < static_cast<int>(m_keyframes.size()) && m_keyframes[i]) return *m_keyframes[i];
	else throw Exception("Invalid keyframe " + lexical_cast<std::string,int>(i));
}

double Animation::length() const
{
	return m_length;
}

}
