/***
 * hesperus: MathUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MathUtil.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
RBTQuaternion_Ptr MathUtil::rbt_matrix_to_quaternion(const RBTMatrix_Ptr& mat)
{
	const RBTMatrix& m = *mat;
	Vector3d trans(m(0,3), m(1,3), m(2,3));

	// NYI
	throw 23;
}

RBTMatrix_Ptr MathUtil::rbt_quaternion_to_matrix(const RBTQuaternion_Ptr& q)
{
	RBTMatrix_Ptr ret = RBTMatrix::zeros();
	RBTMatrix& m = *ret;

	const Quaternion& rot = q->rotation();
	const Vector3d& trans = q->translation();

	const double& w = rot.w;
	const double& x = rot.x;
	const double& y = rot.y;
	const double& z = rot.z;

	// See p.72 of "Mathematics for 3D Game Programming & Computer Graphics" (Eric Lengyel).
	m(0,0) = 1 - 2*y*y - 2*z*z;		m(0,1) = 2*x*y - 2*w*z;			m(0,2) = 2*x*z + 2*w*y;			m(0,3) = trans.x;
	m(1,0) = 2*x*y + 2*w*z;			m(1,1) = 1 - 2*x*x - 2*z*z;		m(1,2) = 2*y*z - 2*w*x;			m(1,3) = trans.y;
	m(2,0) = 2*x*z - 2*w*y;			m(2,1) = 2*y*z - 2*w*x;			m(2,2) = 1 - 2*x*x - 2*y*y;		m(2,3) = trans.z;

	return ret;
}

}
