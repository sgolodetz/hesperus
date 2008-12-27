/***
 * hesperus: Light.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSP_LIGHT
#define H_HESP_BSP_LIGHT

#include <source/math/vectors/Vector3.h>

namespace hesp {

/**
This struct represents point light sources.
*/
struct Light
{
	//#################### PUBLIC VARIABLES ####################
	Vector3d position;
	// TODO: colour, intensity, etc.

	//#################### CONSTRUCTORS ####################
public:
	Light(const Vector3d& position_) : position(position_) {}
};

}

#endif
