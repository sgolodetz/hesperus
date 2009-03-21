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
	static RBTQuaternion_Ptr matrix_to_quaternion(const RBTMatrix_Ptr& m);
	static RBTMatrix_Ptr quaternion_to_matrix(const RBTQuaternion_Ptr& q);
};

}

#endif
