/***
 * hesperus: LineSegment.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATH_GEOM_LINESEGMENT
#define H_HESP_MATH_GEOM_LINESEGMENT

#include <ostream>

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

//#################### GLOBAL OPERATORS ####################
template <typename Vec>
std::ostream& operator<<(std::ostream& os, const LineSegment<Vec>& rhs)
{
	os << "{ " << rhs.e1 << ' ' << rhs.e2 << " }";
	return os;
}

//#################### TYPEDEFS ####################
typedef LineSegment<Vector2d> LineSegment2d;
typedef LineSegment<Vector3d> LineSegment3d;

typedef shared_ptr<LineSegment2d> LineSegment2d_Ptr;
typedef shared_ptr<LineSegment3d> LineSegment3d_Ptr;

}

#endif
