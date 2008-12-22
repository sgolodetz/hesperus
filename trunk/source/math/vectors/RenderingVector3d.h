/***
 * hesperus: RenderingVector3d.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_MATH_VECTORS_RENDERINGVECTOR3D
#define H_HESP_MATH_VECTORS_RENDERINGVECTOR3D

#include "Vector3.h"

namespace hesp {

/**
This struct represents 3D vectors which can also store (u,v) texture coordinates.
*/
struct RenderingVector3d
{
	//#################### PUBLIC VARIABLES ####################
	double x, y, z, u, v;

	//#################### CONSTRUCTORS ####################
	RenderingVector3d();
	RenderingVector3d(double x_, double y_, double z_, double u_, double v_);
	RenderingVector3d(const std::vector<std::string>& components);

	//#################### PUBLIC OPERATORS ####################
	RenderingVector3d& operator+=(const RenderingVector3d& rhs);
	RenderingVector3d& operator-=(const RenderingVector3d& rhs);
	RenderingVector3d& operator*=(double factor);
	operator Vector3d() const;
};

//#################### GLOBAL OPERATORS ####################
RenderingVector3d operator+(const RenderingVector3d& lhs, const RenderingVector3d& rhs);
RenderingVector3d operator-(const RenderingVector3d& lhs, const RenderingVector3d& rhs);
RenderingVector3d operator*(double factor, const RenderingVector3d& v);
std::ostream& operator<<(std::ostream& os, const RenderingVector3d& v);

}

#endif
