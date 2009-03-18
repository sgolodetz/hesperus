/***
 * hesperus: Keyframe.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Keyframe.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Keyframe::Keyframe(const std::vector<RBTMatrix_Ptr>& boneMatrices)
:	m_boneMatrices(boneMatrices)
{}

//#################### PUBLIC METHODS ####################
const std::vector<RBTMatrix_Ptr>& Keyframe::bone_matrices() const
{
	return m_boneMatrices;
}

}
