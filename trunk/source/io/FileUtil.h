/***
 * hesperus: FileUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IO_FILEUTIL
#define H_HESP_IO_FILEUTIL

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/brushes/PolyhedralBrush.h>

namespace hesp {

class FileUtil
{
#if 0
	//#################### LOADING METHODS ####################
public:
	template <typename Poly> static void load_polygons_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons);
#endif
};

}

#include "FileUtil.tpp"

#endif
