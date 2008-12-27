/***
 * TexturedLitVector3d.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_MATH_VECTORS_TEXTUREDLITVECTOR3D
#define H_HESP_MATH_VECTORS_TEXTUREDLITVECTOR3D

namespace hesp {

/**
This struct represents 3D vectors which can also store (u,v) texture coordinates
and (lu,lv) lightmap coordinates.
*/
struct TexturedLitVector3d
{
	//#################### PUBLIC VARIABLES ####################
	double x, y, z, u, v, lu, lv;

	// TODO
};

}

#endif
