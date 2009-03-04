/***
 * hesperus: Matrix44.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Matrix44.h"

#include <cassert>
#include <cmath>

#include <source/exceptions/Exception.h>
#include <source/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Matrix44::Matrix44(double initValue)
:	m(16, initValue)
{}

//#################### STATIC FACTORY METHODS ####################
/**
Constructs a rigid-body transformation matrix from an axis, an angle and a translation vector.

@param axis			The rotation axis
@param angle		The rotation angle (in radians, NOT degrees)
@param translation	The translation vector
*/
Matrix44_Ptr Matrix44::from_axis_angle_translation(Vector3d axis, double angle, const Vector3d& translation)
{
	if(fabs(axis.length() - 1) > SMALL_EPSILON)
	{
		if(axis.length_squared() > EPSILON*EPSILON) axis.normalize();
		else throw Exception("Matrix44::from_axis_angle_translation: Can't rotate about a zero-length axis");
	}

	Matrix44_Ptr ret(new Matrix44);
	Matrix44& mat = *ret;

	double c = cos(angle);
	double s = sin(angle);
	double t = 1 - c;

	const double& x = axis.x;
	const double& y = axis.y;
	const double& z = axis.z;

	// See www.cprogramming.com/tutorial/3d/rotation.html for this matrix (I've derived it in the past and couldn't be bothered to do it again!)
	mat(0,0) = t*x*x + c;		mat(0,1) = t*x*y - s*z;		mat(0,2) = t*x*z + s*y;		mat(0,3) = translation.x;
	mat(1,0) = t*x*y + s*z;		mat(1,1) = t*y*y + c;		mat(1,2) = t*y*z - s*x;		mat(1,3) = translation.y;
	mat(2,0) = t*x*y - s*y;		mat(2,1) = t*y*z + s*x;		mat(2,2) = t*z*z + c;		mat(2,3) = translation.z;
	mat(3,0) = 0;				mat(3,1) = 0;				mat(3,2) = 0;				mat(3,3) = 1;

	return ret;
}

Matrix44_Ptr Matrix44::identity()
{
	Matrix44_Ptr ret(new Matrix44(0));
	Matrix44& mat = *ret;
	mat(0,0) = mat(1,1) = mat(2,2) = mat(3,3) = 1;
	return ret;
}

Matrix44_Ptr Matrix44::zeros()
{
	return Matrix44_Ptr(new Matrix44(0));
}

//#################### PUBLIC OPERATORS ####################
double& Matrix44::operator()(int i, int j)
{
	assert(0 <= i && i < 4 && 0 <= j && j < 4);
	return m[j*4+i];
}

double Matrix44::operator()(int i, int j) const
{
	assert(0 <= i && i < 4 && 0 <= j && j < 4);
	return m[j*4+i];
}

//#################### PUBLIC METHODS ####################
const double *Matrix44::rep() const
{
	return &m[0];
}

}
