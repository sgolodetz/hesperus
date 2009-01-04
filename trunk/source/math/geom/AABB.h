/***
 * hesperus: AABB.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATH_GEOM_AABB
#define H_HESP_MATH_GEOM_AABB

#include <source/math/vectors/Vector2d.h>
#include <source/math/vectors/Vector3.h>

namespace hesp {

/**
This class template represents axis-aligned bounding boxes.
*/
template <typename Vec>
class AABB
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vec m_minimum, m_maximum;

	//#################### CONSTRUCTORS ####################
public:
	AABB(const Vec& minimum, const Vec& maximum);

	//#################### PUBLIC METHODS ####################
public:
	const Vec& maximum() const;
	const Vec& minimum() const;
	static bool within_range(const AABB& lhs, const AABB& rhs, double tolerance);

	//#################### PRIVATE METHODS ####################
private:
	void check_invariant() const;
};

//#################### TYPEDEFS ####################
typedef AABB<Vector2d> AABB2d;
typedef AABB<Vector3d> AABB3d;

}

#endif
