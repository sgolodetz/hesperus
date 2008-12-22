/***
 * hesperus: VertBuilder.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_MATH_VECTORS_VERTBUILDER
#define H_HESP_MATH_VECTORS_VERTBUILDER

#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <source/exceptions/Exception.h>
#include "RenderingVector3d.h"
#include "Vector3.h"

namespace hesp {

template <typename Vert> struct VertBuilder;

template <> struct VertBuilder<RenderingVector3d>
{
	static RenderingVector3d build_vertex(const std::vector<std::string>& components)
	{
		if(components.size() != 5) throw Exception("Incorrect number of vertex components");

		try
		{
			double x = lexical_cast<double,std::string>(components[0]);
			double y = lexical_cast<double,std::string>(components[1]);
			double z = lexical_cast<double,std::string>(components[2]);
			double u = lexical_cast<double,std::string>(components[3]);
			double v = lexical_cast<double,std::string>(components[4]);
			return RenderingVector3d(x,y,z,u,v);
		}
		catch(bad_lexical_cast&) { throw Exception("One of the vertex components is not a number"); }
	}
};

template <> struct VertBuilder<Vector3d>
{
	static Vector3d build_vertex(const std::vector<std::string>& components)
	{
		if(components.size() != 3) throw Exception("Incorrect number of vertex components");

		try
		{
			double x = lexical_cast<double,std::string>(components[0]);
			double y = lexical_cast<double,std::string>(components[1]);
			double z = lexical_cast<double,std::string>(components[2]);
			return Vector3d(x,y,z);
		}
		catch(bad_lexical_cast&) { throw Exception("One of the vertex components is not a number"); }
	}
};

}

#endif
