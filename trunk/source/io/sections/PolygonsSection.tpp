/***
 * hesperus: PolygonsSection.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <source/io/util/LineIO.h>
#include "SectionUtil.h"

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a polygon section from the specified std::istream.

@param is			The std::istream
@param sectionName	The name of the section in the file (e.g. Polygons, Portals, etc.)
@param polygons		The array into which to read the polygons
*/
template <typename Poly>
void PolygonsSection::load(std::istream& is, const std::string& sectionName, std::vector<shared_ptr<Poly> >& polygons)
{
	LineIO::read_checked_line(is, sectionName);
	LineIO::read_checked_line(is, "{");
	SectionUtil::load_counted_polygons(is, polygons);
	LineIO::read_checked_line(is, "}");
}

//#################### SAVING METHODS ####################
/**
Saves a polygon section to the specified std::ostream.

@param os			The std::ostream
@param sectionName	The name to give the section in the file (e.g. Polygons, Portals, etc.)
@param polygons		The array of polygons
*/
template <typename Poly>
void PolygonsSection::save(std::ostream& os, const std::string& sectionName, const std::vector<shared_ptr<Poly> >& polygons)
{
	os << sectionName << '\n';
	os << "{\n";
	SectionUtil::write_polygons(os, polygons, true);
	os << "}\n";
}

}
