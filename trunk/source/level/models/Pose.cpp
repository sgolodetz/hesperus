/***
 * hesperus: Pose.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Pose.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Pose::Pose(const std::vector<RBTMatrix_Ptr>& boneMatrices)
:	m_boneMatrices(boneMatrices)
{}

//#################### PUBLIC METHODS ####################
const std::vector<RBTMatrix_Ptr>& Pose::bone_matrices() const
{
	return m_boneMatrices;
}

Pose_Ptr Pose::interpolate(const Pose_Ptr& lhs, const Pose_Ptr& rhs, double t)
{
	// NYI
	throw 23;
}

}
