/***
 * hesperus: Pose.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Pose.h"

#include <source/math/MathUtil.h>

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
	const std::vector<RBTMatrix_Ptr>& lhsBoneMatrices = lhs->bone_matrices();
	const std::vector<RBTMatrix_Ptr>& rhsBoneMatrices = rhs->bone_matrices();

	assert(lhsBoneMatrices.size() == rhsBoneMatrices.size());

	int boneCount = static_cast<int>(lhsBoneMatrices.size());
	std::vector<RBTMatrix_Ptr> boneMatrices(boneCount);

	for(int i=0; i<boneCount; ++i)
	{
		const RBTMatrix_Ptr& m1 = lhsBoneMatrices[i];
		const RBTMatrix_Ptr& m2 = rhsBoneMatrices[i];
		RBTQuaternion_Ptr q1 = MathUtil::rbt_matrix_to_quaternion(m1);
		RBTQuaternion_Ptr q2 = MathUtil::rbt_matrix_to_quaternion(m2);
		RBTQuaternion_Ptr inter = RBTQuaternion::interpolate(q1, q2, t);
		boneMatrices[i] = MathUtil::rbt_quaternion_to_matrix(inter);
	}

	return Pose_Ptr(new Pose(boneMatrices));
}

}
