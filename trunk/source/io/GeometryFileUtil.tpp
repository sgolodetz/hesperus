/***
 * hesperus: GeometryFileUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>

#include <source/io/util/SectionUtil.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an array of polygons from the specified file.

@param filename		The name of the file
@param polygons		Used to return the polygons to the caller
*/
template <typename Poly>
void GeometryFileUtil::load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");
	SectionUtil::load_uncounted_polygons(is, polygons);
}

//#################### SAVING METHODS ####################
/**
Saves an array of polygons to the specified file.

@param filename		The name of the file
@param polygons		The polygons
*/
template <typename Poly>
void GeometryFileUtil::save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open" + filename + " for writing");
	SectionUtil::write_polygons(os, polygons, false);
}

}
