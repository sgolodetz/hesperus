/***
 * hesperus: GeometryFileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_GEOMETRYFILEUTIL
#define H_HESP_GEOMETRYFILEUTIL

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

struct GeometryFileUtil
{
	//#################### LOADING METHODS ####################
	template <typename Poly> static void load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons);

	//#################### SAVING METHODS ####################
	template <typename Poly> static void save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons);
};

}

#include "GeometryFileUtil.tpp"

#endif
