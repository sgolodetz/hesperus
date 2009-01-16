/***
 * hesperus: LineSegment.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATH_GEOM_LINESEGMENT
#define H_HESP_MATH_GEOM_LINESEGMENT

#include <source/math/vectors/Vector2d.h>
#include <source/math/vectors/Vector3.h>

namespace hesp {

template <typename Vec>
struct LineSegment
{
	//#################### PUBLIC VARIABLES ####################
	Vec e1, e2;		// endpoints

	//#################### CONSTRUCTORS ####################
	LineSegment(const Vec& e1_, const Vec& e2_)
	:	e1(e1_), e2(e2_)
	{}
};

//#################### TYPEDEFS ####################
typedef LineSegment<Vector2d> LineSegment2d;
typedef LineSegment<Vector3d> LineSegment3d;

}

#endif
