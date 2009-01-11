/***
 * hesperus: FileUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>

#include <boost/lexical_cast.hpp>

#include <source/math/geom/GeomUtil.h>
#include "FileSectionUtil.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads an array of polygons from the specified file.

@param filename		The name of the file
@param polygons		Used to return the polygons to the caller
*/
template <typename Poly>
void FileUtil::load_polygons_file(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("The polygons file could not be read");

	FileSectionUtil::load_uncounted_polygons(is, polygons);
}

}
