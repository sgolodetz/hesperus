/***
 * hesperus: MathUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATHUTIL
#define H_HESP_MATHUTIL

#include <source/math/matrices/RBTMatrix.h>
#include <source/math/quaternions/RBTQuaternion.h>

namespace hesp {

struct MathUtil
{
	//#################### PUBLIC METHODS ####################
	static RBTQuaternion_Ptr rbt_matrix_to_quaternion(const RBTMatrix_Ptr& mat);
	static RBTMatrix_Ptr rbt_quaternion_to_matrix(const RBTQuaternion_Ptr& q);
};

}

#endif
