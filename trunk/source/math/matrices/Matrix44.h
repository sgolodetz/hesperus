/***
 * hesperus: Matrix44.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATRIX44
#define H_HESP_MATRIX44

#include <source/math/vectors/Vector3.h>

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class Matrix44> Matrix44_Ptr;
typedef shared_ptr<const class Matrix44> Matrix44_CPtr;

class Matrix44
{
	// TODO

	//#################### CONSTRUCTORS ####################
private:
	Matrix44();

	//#################### STATIC FACTORY METHODS ####################
public:
	static Matrix44_Ptr from_axis_angle(const Vector3d& axis, double angle);
	static Matrix44_Ptr identity();
	static Matrix44_Ptr zeros();

	// TODO
};

}

#endif
