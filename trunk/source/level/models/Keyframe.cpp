/***
 * hesperus: Keyframe.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Keyframe.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Keyframe::Keyframe(const std::vector<Matrix44_CPtr>& boneMatrices)
:	m_boneMatrices(boneMatrices)
{}

//#################### PUBLIC METHODS ####################
const std::vector<Matrix44_CPtr>& Keyframe::bone_matrices() const
{
	return m_boneMatrices;
}

}
