/***
 * hesperus: Light.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL_LIGHTING_LIGHT
#define H_HESP_LEVEL_LIGHTING_LIGHT

#include <source/colours/Colour3d.h>
#include <source/math/vectors/Vector3.h>

namespace hesp {

/**
This struct represents point light sources.
*/
struct Light
{
	//#################### PUBLIC VARIABLES ####################
	Colour3d colour;
	Vector3d position;

	//#################### CONSTRUCTORS ####################
public:
	Light(const Vector3d& position_, const Colour3d& colour_) : position(position_), colour(colour_) {}
};

}

#endif
