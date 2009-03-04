/***
 * hesperus: Matrix44.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATRIX44
#define H_HESP_MATRIX44

#include <vector>

#include <source/math/vectors/Vector3.h>

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class Matrix44> Matrix44_Ptr;
typedef shared_ptr<const class Matrix44> Matrix44_CPtr;

class Matrix44
{
	//#################### PRIVATE VARIABLES ####################
private:
	// Note: The internal array is ordered in column-major order in order to inter-operate easily with OpenGL.
	std::vector<double> m;

	//#################### CONSTRUCTORS ####################
private:
	Matrix44(double initValue = 0);

	//#################### STATIC FACTORY METHODS ####################
public:
	static Matrix44_Ptr from_axis_angle_translation(Vector3d axis, double angle, const Vector3d& translation);
	static Matrix44_Ptr identity();
	static Matrix44_Ptr zeros();

	//#################### PUBLIC OPERATORS ####################
public:
	double& operator()(int i, int j);
	double operator()(int i, int j) const;

	//#################### PUBLIC METHODS ####################
public:
	const double *rep() const;
};

}

#endif
