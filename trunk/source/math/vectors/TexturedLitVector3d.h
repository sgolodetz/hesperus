/***
 * TexturedLitVector3d.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_MATH_VECTORS_TEXTUREDLITVECTOR3D
#define H_HESP_MATH_VECTORS_TEXTUREDLITVECTOR3D

#include "Vector3.h"

namespace hesp {

/**
This struct represents 3D vectors which can also store (u,v) texture coordinates
and (lu,lv) lightmap coordinates.
*/
struct TexturedLitVector3d
{
	//#################### PUBLIC VARIABLES ####################
	double x, y, z, u, v, lu, lv;

	//#################### CONSTRUCTORS ####################
	TexturedLitVector3d(double x_, double y_, double z_, double u_, double v_, double lu_, double lv_)
	:	x(x_), y(y_), z(z_), u(u_), v(v_), lu(lu_), lv(lv_)
	{}

	//#################### PUBLIC OPERATORS ####################
	operator Vector3d() const;
};

}

#endif