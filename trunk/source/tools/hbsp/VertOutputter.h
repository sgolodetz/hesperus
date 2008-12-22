/***
 * hbsp: VertOutputter.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HBSP_VERTOUTPUTTER
#define H_HBSP_VERTOUTPUTTER

#include <ostream>

#include <source/math/vectors/Vector3.h>
#include "RenderingVector3d.h"

namespace hesp {

template <typename Vert> struct VertOutputter;

template <> struct VertOutputter<Vector3d>
{
	static void output(std::ostream& os, const Vector3d& v)
	{
		os << "( " << v.x << ' ' << v.y << ' ' << v.z << " )";
	}
};

template <> struct VertOutputter<RenderingVector3d>
{
	static void output(std::ostream& os, const RenderingVector3d& v)
	{
		os << "( " << v.x << ' ' << v.z << ' ' << v.z << ' ' << v.u << ' ' << v.v << " )";
	}
};

}

#endif
