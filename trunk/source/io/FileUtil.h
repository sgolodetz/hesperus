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
	//#################### LOADING METHODS ####################
public:
	template <typename Poly> static void load_polygons_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static void load_separator(std::istream& is);
};

}

#include "FileUtil.tpp"

#endif
