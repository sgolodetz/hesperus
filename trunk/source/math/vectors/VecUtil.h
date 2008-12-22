/***
 * hesperus: VecUtil.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_MATH_VECTORS_VECUTIL
#define H_HESP_MATH_VECTORS_VECUTIL

#include <ostream>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include "RenderingVector3d.h"
#include "Vector3.h"

namespace hesp {

template <typename Vec> struct VecUtil;

template <> struct VecUtil<RenderingVector3d>
{
	static RenderingVector3d build_vector(const std::vector<std::string>& components)
	{
		if(components.size() != 5) throw Exception("Incorrect number of vector components");

		try
		{
			double x = lexical_cast<double,std::string>(components[0]);
			double y = lexical_cast<double,std::string>(components[1]);
			double z = lexical_cast<double,std::string>(components[2]);
			double u = lexical_cast<double,std::string>(components[3]);
			double v = lexical_cast<double,std::string>(components[4]);
			return RenderingVector3d(x,y,z,u,v);
		}
		catch(bad_lexical_cast&) { throw Exception("One of the vector components is not a number"); }
	}

	static void output_vector(std::ostream& os, const RenderingVector3d& v)
	{
		os << "( " << v.x << ' ' << v.z << ' ' << v.z << ' ' << v.u << ' ' << v.v << " )";
	}
};

template <> struct VecUtil<Vector3d>
{
	static Vector3d build_vector(const std::vector<std::string>& components)
	{
		if(components.size() != 3) throw Exception("Incorrect number of vector components");

		try
		{
			double x = lexical_cast<double,std::string>(components[0]);
			double y = lexical_cast<double,std::string>(components[1]);
			double z = lexical_cast<double,std::string>(components[2]);
			return Vector3d(x,y,z);
		}
		catch(bad_lexical_cast&) { throw Exception("One of the vector components is not a number"); }
	}

	static void output_vector(std::ostream& os, const Vector3d& v)
	{
		os << "( " << v.x << ' ' << v.y << ' ' << v.z << " )";
	}
};

}

#endif
