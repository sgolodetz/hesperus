/***
 * hesperus: VertOutputter.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_MATH_VECTORS_VERTOUTPUTTER
#define H_HESP_MATH_VECTORS_VERTOUTPUTTER

#include <ostream>

#include "RenderingVector3d.h"
#include "Vector3.h"

namespace hesp {

template <typename Vert> struct VertOutputter;

template <> struct VertOutputter<RenderingVector3d>
{
	static void output(std::ostream& os, const RenderingVector3d& v)
	{
		os << "( " << v.x << ' ' << v.z << ' ' << v.z << ' ' << v.u << ' ' << v.v << " )";
	}
};

template <> struct VertOutputter<Vector3d>
{
	static void output(std::ostream& os, const Vector3d& v)
	{
		os << "( " << v.x << ' ' << v.y << ' ' << v.z << " )";
	}
};

}

#endif
